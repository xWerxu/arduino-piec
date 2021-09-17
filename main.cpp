#include <Arduino.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, value = 1;

void timerIsr() {
  encoder->service();
}


void setup() {

  Serial.begin(9600);
  encoder = new ClickEncoder(5, 6, 8, 2 ); //PINS:  DT, CLK, SW (button), STEP (Number of steps when rotating encoder)

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
 
  last = -1;
}

int temp1 = 0; // Pierwszy próg temperatury, wyłącza się pierwszy zawór
int temp2 = 0; // Drugi próg temperatury, wyłączają się oba zawory
int temp3 = 0; // Załącza się syrena alarmowa, nie wyłącza się do momentu interakcji z programem

int chg_value = 0;
int chg_last = 0;

bool main_menu = 1;
bool temp_done = 0;
bool chg_done = 0;
bool zawor_done = 0;

void chg_temp();
void chg_zawor1();
void chg_zawor2();
void chg_syrena();
void chg_temp_menu();



void menu_print()
{

    while (main_menu == 1)
    {
      value += encoder->getValue();
      switch (value)
      {
        case 1:
        case 3:
        Serial.println(">Temp i zawory");
        Serial.println(" Ustawienia temp");
        break;

        case 2:
        case 4:
        Serial.println(" Temp i zawory");
        Serial.println(">Ustawienia temp");
        break;

        default:
        break;
      }

      break;

    }

}


void show_temp()
{
  temp_done = 0;
  main_menu = 0;

  while(temp_done == 0)
  {

    if((millis()/1000) % 5 == 0)
    {
      Serial.println("Temper : 26C");
      Serial.println("Zawor 1: ON  60C");
      Serial.println("Zawor 2: OFF 80C");
      Serial.println("Syrena : OFF 100C");
    }

    if(encoder->getButton() == ClickEncoder::Clicked)
    {
      temp_done = 1;
      main_menu = 1;
      menu_print();
    }
  }
  
  
}

void chg_zawor1()
{
  chg_done = 0;
  temp1 = chg_value;
  while (chg_done == 0)
  {
    chg_value += encoder->getValue();
    if (chg_value != chg_last) {

      chg_last = chg_value;
      if(chg_value < 0)
      {
        chg_value = 0;
      }
    
    Serial.println(chg_value);


    }
    



    if(encoder->getButton() == ClickEncoder::Clicked)
    {
      temp1 = chg_value;
      chg_done = 1;
      chg_temp();
    }

  }
    
}

void chg_zawor2()
{

}

void chg_syrena()
{

}


void chg_temp_menu()
{
  chg_done = 0;
  main_menu = 0;

while (main_menu == 0 && chg_done == 0)
    {
      switch (value)
      {
        case 1:
        Serial.println(">Próg zaworu 1");
        Serial.println(" Próg zaworu 2");
        Serial.println(" Próg syreny");
        break;

        case 2:
        Serial.println(" Próg zaworu 1");
        Serial.println(">Próg zaworu 2");
        Serial.println(" Próg syreny");
        break;

        case 3:
        Serial.println(" Próg zaworu 1");
        Serial.println(" Próg zaworu 2");
        Serial.println(">Próg syreny");
        break;

        
        default:
        break;
      }

      break;

    if(encoder->getButton() == ClickEncoder::Clicked)
    {
      chg_done = 1;
      main_menu = 1;
      menu_print();
    }

    }


}

void chg_temp()
{
  chg_done = 0;
  main_menu = 0;

  chg_temp_menu();

  while(chg_done == 0)
  {
    value += encoder->getValue();
    if (value != last)
    {

     last = value;

      if(value > 3)
    {
      value = 1;
      last = 1;

    }else if (value < 1)
    {
      value = 3;
      last = 3;
    }

    chg_temp_menu();

    }
    
    
    

    if(encoder->getButton() == ClickEncoder::Clicked)
    {

      switch (value)
      {
        case 1:
        chg_zawor1();
        break;

        case 2:
        chg_zawor2();
        break;

        case 3:
        chg_syrena();
        break;

        case 4:
        chg_done = 1;
        main_menu = 1;
        break;


        default:
        break;
      }

    }
  }
  
  
}





void loop() { 
  value += encoder->getValue();
 
  if (value != last) {
    
    last = value;

      if(value > 4)
    {
      value = 1;
      last = 1;

    }else if (value < 1)
    {
      value = 4;
      last = 4;
    }

    // Serial.print("Encoder Value: ");
    // Serial.println(value);
    menu_print();
  }

   ClickEncoder::Button buttonEnc = encoder->getButton();

  if (main_menu == 1 && buttonEnc == ClickEncoder::Clicked)
  {
    switch (value)
    {
    case 1:
    case 3:
      show_temp();
      break;
    
    case 2:
    case 4:
      chg_temp();
      break;
    default:
      break;
    }

 
  }

 // if (buttonEnc != ClickEncoder::Open) {
  //   Serial.print("Button: ");
  //   switch (buttonEnc) {

  //     case ClickEncoder::Clicked:
  //         Serial.println("Clicked");
  //         break;

  //   }
    
  // }  

    

  
}
