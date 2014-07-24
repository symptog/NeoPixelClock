/**************************************************************************
 *                                                                     	*  	 
 *   NeoPixel Ring Clock                                               	*
 *   Copyright (C) 2014  Andy Doro (mail@andydoro.com)                 	*
 *                                                                     	*
 ***************************************************************************
 *                                                                     	*
 * This program is free software; you can redistribute it and/or modify	*
 * it under the terms of the GNU General Public License as published by	*
 * the Free Software Foundation; either version 2 of the License, or   	*
 * (at your option) any later version.                                 	*
 *                                                                     	*
 * This program is distributed in the hope that it will be useful,     	*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of      	*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       	*
 * GNU General Public License for more details.                        	*
 *                                                                     	*
 * You should have received a copy of the GNU General Public License   	*
 * along with this program; if not, write to the Free Software         	*
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,               	*
 * MA  02111-1307  USA                                                 	*
 *                                                                     	*
 ***************************************************************************
 *
 *
 * Revision History
 *
 * Date 	 By    What
 * 20140320 	AFD 	First draft
 */

// include the library code:
#include <Wire.h>
//#include <DS1307RTC.h>
//#include <Time.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

// define pins
#define NEOPIN 6

#define BRIGHTNESS 24 // set max brightness

RTC_DS1307 RTC; // Establish clock object
DateTime Clock; // Holds current clock time

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEOPIN, NEO_GRB + NEO_KHZ800); // strip object

byte hourval, minuteval, secondval; // holds the time

byte pixelColorRed, pixelColorGreen, pixelColorBlue; // holds color values

void setup () {
  Wire.begin();        	// Begin I2C
  RTC.begin();   // begin clock

  Serial.begin(9600);
  // set pinmodes
  pinMode(NEOPIN, OUTPUT);

  if (! RTC.isrunning()) {
	Serial.println("RTC is NOT running!");
	// following line sets the RTC to the date & time this sketch was compiled
	RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  strip.begin();
  //strip.show(); // Initialize all pixels to 'off'

  strip.setBrightness(BRIGHTNESS); // set brightness
}

void loop () {


  char* colon = ":"; // static characters save a bit
  char* slash = "/"; // of memory

  // get time
  Clock = RTC.now(); // get the RTC time

  secondval = Clock.second();  // get seconds
  minuteval = Clock.minute();  // get minutes
  hourval = Clock.hour();  	// get hours
  if(hourval > 11) hourval -= 12; // This clock is 12 hour, if 13-23, convert to 0-11


  hourval = (hourval*60 + minuteval) / 12;  //each red dot represent 24 minutes.

  clearAll();
  setHours();

  strip.setPixelColor(hourval, strip.Color(199, 21, 133));
  strip.setPixelColor(minuteval, strip.Color(0, 255, 0));
  strip.setPixelColor(secondval, strip.Color(30, 144, 255));
  
  if (hourval == minuteval) {
    strip.setPixelColor(hourval, strip.Color(255, 255, 0));
  }
  
  // for serial debugging
   Serial.print(hourval, DEC);
   Serial.print(':');
   Serial.print(minuteval, DEC);
   Serial.print(':');
   Serial.println(secondval, DEC);

  //display
  strip.show();
}

void clearAll() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
}

void setHours() {
  for(uint16_t i=0; i<strip.numPixels(); i+=5) {
    strip.setPixelColor(i, strip.Color(255, 255, 139));
  }
}
