#include <Arduino.h>
#include "BluetoothSerial.h"
#include "AppDebug.h"
#include "ArduinoJson.h"
#include "soc/soc.h"  //Brownout detector was triggered
#include "soc/rtc_cntl_reg.h"

#define MAX_RESPONSE_LENGTH     2048


BluetoothSerial SerialBT;
void bluetoothInit();
void callbackBluetooth(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

void bluetoothInit()
{
    SerialBT.flush();
    SerialBT.end(); 
    if(!SerialBT.begin("Tranh 1")){
        ECHOLN("An error occurred initializing Bluetooth");
    }else{
        ECHOLN("Bluetooth initialized");
    }
  SerialBT.register_callback(callbackBluetooth);
}

void callbackBluetooth(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  switch (event)
  {
  case ESP_SPP_SRV_OPEN_EVT:
    ECHOLN("Client Connected");
    break;
    case ESP_SPP_CLOSE_EVT:
      ECHOLN("Client Disconnected");
      // sendDatatoAppTicker.stop();
    break;
  case ESP_SPP_DATA_IND_EVT:  
    if (param->data_ind.len < MAX_RESPONSE_LENGTH) 
      {
        String data;
        for(int i = 0; i < param->data_ind.len; i++)
        {
            data += (char)param->data_ind.data[i];
        }
        ECHO("data bluetooth: ");
        ECHOLN(data);
        StaticJsonBuffer<MAX_RESPONSE_LENGTH> jsonBuffer;
        JsonObject& rootData = jsonBuffer.parseObject(data);
        if(rootData.success())
        {
        }
      }
      break;
  default:
    break;
  }
}

void setup() {
  // put your setup code here, to run once:
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //Brownout detector was triggered
  Serial.begin(115200);
  bluetoothInit();
}

void loop() {
  // put your main code here, to run repeatedly:
}