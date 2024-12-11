#include <LiquidCrystal.h>

LiquidCrystal lcd(1,2,4,5,6,7);
const int buzzer = 8;
int buttonState = 0;
bool lastButtonState = false;
unsigned long endPressed = 0;
unsigned long startPressed = 0;
int holdTime = 0;
String buff = "";

void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(buzzer, INPUT_PULLUP);  // Changed to INPUT_PULLUP
}

void loop() {
  buttonState = digitalRead(buzzer);
  
  if (buttonState == LOW) {  // Button is pressed (LOW when using INPUT_PULLUP)
    if (millis() - endPressed > 500) {
      if (buff != "") {
        Serial.println(retBuf());
        clearBuf();
        lastButtonState = true;
      }
    }
    
    if (millis() - endPressed > 1500) {
      if (lastButtonState) {
        Serial.println("SPACE HERE");
        lastButtonState = false;
      }
    }
  }
  else {  // Button is not pressed (HIGH when using INPUT_PULLUP)
    static unsigned long lastCheckTime = 0;
    if (millis() - lastCheckTime >= 50) {  // Debounce
      updateState();
      lastCheckTime = millis();
    }
  }
}

void updateState() {
  startPressed = millis();
  
  // Wait for button release with non-blocking method
  while (digitalRead(buzzer) == HIGH) {
    // Optional: Add a short delay to reduce CPU usage
    delay(10);
  }
  
  endPressed = millis();
  holdTime = endPressed - startPressed;
  
  Serial.println(holdTime);
  
  if (holdTime < 500) {
    Serial.println("DIT");
    buff += ".";
  }
  else {
    Serial.println("DAH");
    buff += "-";
  }
}

String retBuf() {
  return morseToChar(buff);
}

String morseToChar(const String& morse) {
  static String letters[] = {".-","-...","-.-","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
  
  for (int k = 0; k < (sizeof(letters)/sizeof(letters[0])); k++) {
    if (letters[k] == morse) {
      lcd.print((char)(k+65));
      return String((char)(k+65));
    }
  }
  return "ERROR";
}

void clearBuf() {
  buff = "";
}