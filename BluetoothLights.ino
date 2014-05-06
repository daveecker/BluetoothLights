/*
4/1/2014
SpeechBlinker sketch
modified from the Simple Controls sketch provided by RedBearLab in their nRF8001 library
*/

//"services.h/spi.h/boards.h" is needed in every new project
#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include <services.h>
 
#define BLUE        2
#define GREEN       3
#define RED         4
#define LED_GROUND  7

void setup()
{
  // Default pins set to 9 and 8 for REQN and RDYN
  // Set your REQN and RDYN here before ble_begin() if you need
  //ble_set_pins(3, 2);
  
  // Set your BLE Shield name here, max. length 10
  //ble_set_name("My Name");
  
  // Init. and start BLE library.
  ble_begin();
  
  // Enable serial debug
  Serial.begin(57600);
  
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_GROUND, OUTPUT);
  
  // make sure all pins used are high, which turns the LED off and disables it
  digitalWrite(LED_GROUND, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(GREEN, HIGH);
}

void loop()
{
  static byte old_state = LOW;
  
  // If data is ready
  while(ble_available())
  {
    // read out command and data
    byte data0 = ble_read();
    byte data1 = ble_read();
    byte data2 = ble_read();
    
    /*
    Light commands structure looks like this:
    000 - enable LED (set common cathode to high)
    010 - disable LED (set common cathode to low)
    100 - no lights (all signal bits high)
    110 - red on
    120 - red off
    130 - green on
    140 - green off
    150 - blue on
    160 - blue off
    170 - all lights on
    */
    
    if (data0 == 0x00)  // This is a command to enable the LED by setting the common-cathode pin to ground
    {
      if (data1 == 0x01)
        digitalWrite(LED_GROUND, LOW);
      else
        digitalWrite(LED_GROUND, HIGH);
    }
    else if (data0 == 0x01) // Command is to change the lights
    {
      // turn all lights off
      if(data1 == 0x00){
          digitalWrite(RED, HIGH);
          digitalWrite(GREEN, HIGH);
          digitalWrite(BLUE, HIGH);
      }
      // red on
      if (data1 == 0x01){
        digitalWrite(RED, LOW);
        //digitalWrite(GREEN, HIGH);
        //digitalWrite(BLUE, HIGH);
      }
      //  red off   
      else if(data1 == 0x02){
        digitalWrite(RED, HIGH);
        //digitalWrite(BLUE, HIGH);
        //digitalWrite(RED, HIGH);
      }
      // green on
      else if(data1 == 0x03){
        //digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);
        //digitalWrite(BLUE, HIGH);
      }
      // green off
      else if(data1 == 0x04){
        //digitalWrite(RED, LOW);
        digitalWrite(GREEN, HIGH);
        //digitalWrite(BLUE, HIGH);
      }
      // blue on
      else if(data1 == 0x05){
        //digitalWrite(RED, HIGH);
        //digitalWrite(GREEN, HIGH);
        digitalWrite(BLUE, LOW);
      }
      // blue off
      else if(data1 == 0x06){
        //digitalWrite(RED, LOW);
        //digitalWrite(GREEN, HIGH);
        digitalWrite(BLUE, HIGH);
      }
      // all lights on
      else if(data1 == 0x07){
        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, HIGH);
        digitalWrite(BLUE, HIGH);
      }
    }
  }
  
  if (!ble_connected())
  {
    // turn off the lights when disconnected
    digitalWrite(LED_GROUND, HIGH);
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, HIGH);
  }
  
  // Allow BLE Shield to send/receive data
  ble_do_events();  
}



