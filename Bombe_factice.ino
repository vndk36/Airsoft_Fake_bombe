/* @file EventSerialKeypad.pde
 || @version 1.0
 || @author Alexander Brevig
 || @contact alexanderbrevig@gmail.com
 ||
 || @description
 || | Demonstrates using the KeypadEvent.
 || #
 */
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <EEPROM.h>

const int ledPin = LED_BUILTIN;  // the pin with a LED
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
    {1,   2, 3, 'A'},
    {4,   5, 6, 'B'},
    {7,   8, 9, 'C'},
    {'*', 0,'#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
//----------------------------------------------------------------------------------------------------------------------------------
/*----------------------------------------------------------------------------------------------------------------------------------
 * 
 * Globale Variable that will be used through the all code
 */

unsigned int timeLeft[2] = {0,0}; 
boolean blink = false;
boolean ledPin_state;
volatile int blinkCount = 0; // use volatile for shared variables

//object init for i/o hardware
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//------------------------------------------------------------------------------------------------------------------------------------

void setup(){
    lcd.init();                           // initialize the lcd 
    lcd.backlight();
    pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
    digitalWrite(ledPin, HIGH);           // Turn the LED on.
    ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
    Timer1.initialize(1000000);
    Timer1.attachInterrupt(timerISR); // blinkLED to run every 0.15 seconds
    //Timer1.stop();
    Serial.begin(57600);
    EEPROM.get(1, blinkCount);
    EEPROM.put(1, 124);
}

int ledState = LOW;

void loop(){
    char key = keypad.getKey();
    delay(100);
    lcd.setCursor(0,1);
    char counterString [20];
    dtostrf(blinkCount,0,4,counterString);
    lcd.write(counterString);
    if (key) {
        Serial.println(key);
        lcd.setCursor(0,0);
        lcd.write(key);
    }
    if (blink){
        digitalWrite(ledPin,!digitalRead(ledPin));    // Change the ledPin from Hi2Lo or Lo2Hi.
        delay(100);
    }
}


void timeToMinutes(void){
  
  }

void timeToSeconds(void){
  
  }


// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED:
        if (key == '#') {
            digitalWrite(ledPin,!digitalRead(ledPin));
            ledPin_state = digitalRead(ledPin);        // Remember LED state, lit or unlit.
        }
        break;

    case RELEASED:
        if (key == '*') {
            digitalWrite(ledPin,ledPin_state);    // Restore LED state from before it started blinking.
            blink = false;
        }
        break;

    case HOLD:
        if (key == '*') {
            blink = true;    // Blink the LED when holding the * key.
        }
        break;
    }
}

void timerISR(void)
{
  Serial.println(blinkCount);
  blinkCount = blinkCount + 1;
  
  if (ledState == LOW) {
    ledState = HIGH;    
  } else {
    ledState = LOW;
  }
  digitalWrite(ledPin, ledState);
}
