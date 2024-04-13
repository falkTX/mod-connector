// SPDX-FileCopyrightText: 2024 Filipe Coelho <falktx@darkglass.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "host.hpp"
#include "lv2.hpp"
#include "websocket.hpp"

#include <QCoreApplication>
#include <cstdint>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("mod-connector");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Darkglass");

    Host host;

    if (! host.last_error.empty())
    {
        fprintf(stderr, "Failed to initialize host connection: %s\n", host.last_error.c_str());
        return 1;
    }

    const std::string ptest = host.preset_show("file:///usr/lib/lv2/sorcerBasicBass.lv2/sorcerBasicBass.ttl");
    if (ptest.empty())
        fprintf(stderr, "preset_show error '%s'\n", host.last_error.c_str());
    else
        fprintf(stdout, "preset_show resp '%s'\n", ptest.c_str());

    fprintf(stdout, "cpu_load resp '%f'\n", host.cpu_load());
    fprintf(stdout, "cpu_load resp '%f'\n", host.cpu_load());
    fprintf(stdout, "cpu_load resp '%f'\n", host.cpu_load());

    WebSocket websocket;
    if (! websocket.listen(13371))
    {
        fprintf(stderr, "Failed to initialize websocket connection: %s\n", websocket.last_error.c_str());
        return 1;
    }

    Lv2World lv2world;
    const uint32_t pcount = lv2world.get_plugin_count();
    for (uint32_t i=0; i<pcount; ++i)
    {
        const Lv2Plugin* const plugin = lv2world.get_plugin(i);
        if (plugin == nullptr)
            continue;
        fprintf(stdout, "plugin %d/%d:\n", i + 1, pcount);
        fprintf(stdout, "\turi: %s\n", plugin->uri.c_str());
        fprintf(stdout, "\tname: %s\n", plugin->name.c_str());
    }

    return app.exec();
}
