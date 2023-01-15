#include <RCSwitch.h>


RCSwitch mySwitch = RCSwitch();


int BTN_READ = 9;
int BTN_PLAY = 7;
long CODIGO = -1;
int ESTADO_ACTUAL = 1;
int bytesCodigo = 0;

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define I2C_ADDRESS 0x3C
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;

void setup() {

  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_READ, INPUT_PULLUP);
 
  Serial.begin(9600);
 
  
  #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif 

  oled.setFont(Adafruit5x7);
  oled.set2X(); 
  oled.displayRemap(true);
  oled.clear();
  oled.print("rf433 pwner");

  delay(1000);       
}

void loop() {


  if (ESTADO_ACTUAL == 1){
    
    if (digitalRead(BTN_READ)==LOW){

      Serial.println("HAS PULSADO EL BOTON DE ESCUCHAR");

      mySwitch.enableReceive(0); 
      
      oled.clear();     
      oled.println("Escuchando...");
      
      ESTADO_ACTUAL = 2;  
    }
  
  }


  else if (ESTADO_ACTUAL == 2){


    while(1){
      
      Serial.println("ESTOY ESCUCHANDO...");
      
      if (mySwitch.available()) {
            CODIGO = mySwitch.getReceivedValue();
            bytesCodigo = mySwitch.getReceivedBitlength();
            
            Serial.print("SE HA LEIDO EL CODIGO: ");
            Serial.println(CODIGO);
            Serial.println(bytesCodigo);

            delay(2000);

            oled.clear(); 
            oled.println("Leido:");
            oled.println(CODIGO);
            
            ESTADO_ACTUAL = 3;
            break;       
      }
    }      
  }


  else if (ESTADO_ACTUAL == 3){

      Serial.println("ESTOY A LA ESPERA DE EMITIR O VOLVER A ESCUCHAR");
    
      if (digitalRead(BTN_PLAY)==LOW){
          
          delay(200);
          
          mySwitch.enableTransmit(10);

          Serial.println("EMITO EL CODIGO:");
          Serial.println(CODIGO);
          Serial.println(bytesCodigo);

          delay(500);
          
          oled.clear(); 
          oled.println(CODIGO);
          oled.println("Emitido");
          delay(500);

          mySwitch.send(CODIGO, bytesCodigo);
          
          delay(3000);
          
          oled.clear(); 
          oled.println("Codigo:");
          oled.println(CODIGO);
          
          delay(500);
      }

      if (digitalRead(BTN_READ)==LOW){

        Serial.println("NOS VOLVEMOS AL MODO ESCUCHA");
        
        ESTADO_ACTUAL = 1;
        CODIGO = -1;
        
      }
  
  }

  
}
