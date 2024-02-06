#include "DHT.h"
#include<LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // pin connected to the LCD
#define DHTPIN 12     // Pin connected to the temperature sensor


#define DHTTYPE DHT11   // Define the type of DHT sensor as DHT11

#define pwm 9 // pin connected to control the fan speed

//to display the degree symbold
byte degree[8] = 
              {
                0b00011,
                0b00011,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000
              };
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

//set up for LCD screen
void setup() {
  lcd.begin(16, 2);
 lcd.createChar(1, degree);
 lcd.clear();
 lcd.print("   Fan Speed  ");
 lcd.setCursor(0,1);
 lcd.print("  Controlling ");
 delay(2000);
 analogWrite(pwm, 255);
 lcd.clear();
 lcd.print("Temp sensor ");
 delay(2000);
  Serial.begin(9600); 
  dht.begin();
}



// 2 seconds loop for reading the sensor 
void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  // check if sensor received an error input
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    // Display error message on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed Sensor");
    lcd.setCursor(0, 1);
    lcd.print("Please check Sensor");
    //Turn off fan
    analogWrite(pwm, 0);

    delay(5000); // Display error message for 5 seconds
    return;
  }
  // data display on Serial Monitor
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");

 //Data  display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");

  lcd.setCursor(0, 1);

// if loop function to control the fan according to the set temperature
// if loop funtion to display the fan speed
  if (t < 20) {
    analogWrite(pwm, 0);
    lcd.print("Fan OFF");
  } else if (t >= 20 && t < 26) {
    analogWrite(pwm, 51);
    lcd.print("Fan Speed: 20%");
  } else if (t >= 26 && t < 28) {
    analogWrite(pwm, 102);
    lcd.print("Fan Speed: 40%");
  } else if (t >= 28 && t < 29) {
    analogWrite(pwm, 153);
    lcd.print("Fan Speed: 60%");
  } else if (t >= 29 && t < 30) {
    analogWrite(pwm, 204);
    lcd.print("Fan Speed: 80%");
  } else if (t >= 30) {
    analogWrite(pwm, 255);
    lcd.print("Fan Speed: 100%");
  }
    else{
    analogWrite(pwm,0);
    lcd.print("Fan Speed: 0%");
  }
  delay(5000);
}
