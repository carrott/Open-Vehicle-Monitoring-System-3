/*
;    Project:       Open Vehicle Monitor System
;    Date:          30th September 2017
;
;    Changes:
;    1.0  Initial release
;
;    (C) 2017       Tom Parker
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
*/

#include "esp_log.h"
static const char *TAG = "terriblenetworkmanager";

#include <stdio.h>
#include <string.h>
#include "pcp.h"
#include "terrible_network_manager.h"
#include "ovms_events.h"
#include "ovms_metrics.h"
#include "metrics_standard.h"

bool tnm_connected = false;
uint8_t tnm_wifi_network_counter = 0;

OvmsTerribleNetworkManager MyTerribleNetworkManager __attribute__ ((init_priority (9001)));

void OvmsTerribleNetworkManager::WifiConnect(std::string event, void* data)
  {
  ESP_LOGI(TAG, "wifi connected");
  tnm_connected = true;
  }

void OvmsTerribleNetworkManager::WifiDisconnect(std::string event, void* data)
  {
  ESP_LOGI(TAG, "wifi disconnected");
  tnm_connected = false;
  }

void OvmsTerribleNetworkManager::Tick(std::string event, void* data)
  {
  // TODO make less terrible
  if (!tnm_connected)
    {
    if (tnm_wifi_network_counter % 3 == 0)
      {
      ESP_LOGI(TAG, "wifi connect 0");
      MyEvents.SignalEvent("tnm.connect.0", NULL);
      }
    else if (tnm_wifi_network_counter % 3 == 1)
      {
      ESP_LOGI(TAG, "wifi connect 1");
      MyEvents.SignalEvent("tnm.connect.1", NULL);
      }
    else if (tnm_wifi_network_counter % 3 == 2)
      {
      ESP_LOGI(TAG, "wifi connect 2");
      MyEvents.SignalEvent("tnm.connect.2", NULL);
      }
    }
    tnm_wifi_network_counter++;
  }

OvmsTerribleNetworkManager::OvmsTerribleNetworkManager()
  {
  ESP_LOGI(TAG, "Starting Terrible Network Manager (9000)");

  using std::placeholders::_1;
  using std::placeholders::_2;
  MyEvents.RegisterEvent(TAG, "ticker.60", std::bind(&OvmsTerribleNetworkManager::Tick, this, _1, _2));
  MyEvents.RegisterEvent(TAG, "system.wifi.sta.connected", std::bind(&OvmsTerribleNetworkManager::WifiConnect, this, _1, _2));
  MyEvents.RegisterEvent(TAG, "system.wifi.sta.disconnected", std::bind(&OvmsTerribleNetworkManager::WifiDisconnect, this, _1, _2));
  }

OvmsTerribleNetworkManager::~OvmsTerribleNetworkManager()
  {
  // TODO unbind event handlers?
  }
