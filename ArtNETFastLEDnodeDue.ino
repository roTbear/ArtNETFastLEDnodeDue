#include <SPI.h>
#include <Artnet.h>
#include <FastLED.h>

#define NUM_LEDS_PER_STRIP 300
#define NUM_STRIPS 8

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
ArtnetReceiver artnet;
uint32_t universe1 = 1;
struct universe_s {
  const uint32_t id;
  const bool active;
};
const universe_s uni[NUM_STRIPS] = {
  {10,true},
  {11,true},
  {0,false},
};

void artNetCallBackCommon(uint8_t u_idx, uint8_t* data, uint16_t size) {
        Serial.print("artnet data (universe : ");
        Serial.print(uni[u_idx].id);
        Serial.println(") = ");
        for (size_t i = 0; i < size; ++i)
        {
            Serial.print(data[i]); Serial.print(",");
        }
        Serial.println();
}

void artNetCallBack1(uint8_t* data, uint16_t size) {
  artNetCallBackCommon(0, data, size);
}

void artNetCallBack2(uint8_t* data, uint16_t size) {
  artNetCallBackCommon(1, data, size);
}

void artNetCallBack3(uint8_t* data, uint16_t size) {
  artNetCallBackCommon(2, data, size);
}

void artNetCallBack4(uint8_t* data, uint16_t size) {
  artNetCallBackCommon(3, data, size);
}

void artNetCallBack5(uint8_t* data, uint16_t size) {
  artNetCallBackCommon(4, data, size);
}

void artNetCallBack6(uint8_t* data, uint16_t size) {
  artNetCallBackCommon(5, data, size);
}

void artNetCallBack7(uint8_t* data, uint16_t size) {
  artNetCallBackCommon(6, data, size);
}

void artNetCallBack8(uint8_t* data, uint16_t size) {
  artNetCallBackCommon(7, data, size);
}

// Pin layouts on the due
// WS2811_PORTA: 69,68,61,60,59,100,58,31 (note: pin 100 only available on the digix)
// WS2811_PORTB: 90,91,92,93,94,95,96,97 (note: only available on the digix)
// WS2811_PORTD: 25,26,27,28,14,15,29,11
//

byte mac[] = { 0xDE, 0xAD, 0xBE, 0x2F, 0x1E, 0xE3 };

void setup() {
  delay(5000);
  Serial.begin(115200);
  Serial.println("Starting...");
  Serial.println("Sending DHCP request...");
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed, rebooting....");
    return;
  }
  Serial.print("Success!\nIP: ");
  Serial.println(Ethernet.localIP());
  artnet.begin();
  
  LEDS.addLeds<WS2811_PORTD,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  //LEDS.addLeds<WS2811_PORTD,NUM_STRIPS>((CRGB*)e131.data, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);// if Artnet packet comes to this universe, this function is called
    
//  artnet.subscribe(universe1, [&](uint8_t* data, uint16_t size)
//  {
//      Serial.print("artnet data (universe : ");
//      Serial.print(universe1);
//      Serial.println(") = ");
//      for (size_t i = 0; i < size; ++i)
//      {
//          Serial.print(data[i]); Serial.print(",");
//      }
//      Serial.println();
//  });

  if (uni[0].active == true) artnet.subscribe(uni[0].id, artNetCallBack1);
  if (uni[1].active == true) artnet.subscribe(uni[1].id, artNetCallBack2);
  if (uni[2].active == true) artnet.subscribe(uni[2].id, artNetCallBack3);
  if (uni[3].active == true) artnet.subscribe(uni[3].id, artNetCallBack4);
  if (uni[4].active == true) artnet.subscribe(uni[4].id, artNetCallBack5);
  if (uni[5].active == true) artnet.subscribe(uni[5].id, artNetCallBack6);
  if (uni[6].active == true) artnet.subscribe(uni[6].id, artNetCallBack7);
  if (uni[7].active == true) artnet.subscribe(uni[7].id, artNetCallBack8);

}

void loop() {
  Serial.println("Loop....");
  static uint8_t hue = 0;
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
    }
  }

  // Set the first n leds on each strip to show which strip it is
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j <= i; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB::Red;
    }
  }

  hue++;

  artnet.parse(); // check if artnet packet has come and execute callback
  LEDS.show();
  // LEDS.delay(100);
}
