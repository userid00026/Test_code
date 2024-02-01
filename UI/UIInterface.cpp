#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>

#define KEYDEBOUNCEMASK(x)  (1 << x)
int ledBlinkMenu = 0;
bool bLedReset = false;

enum SYSTEMSTATE{
	SYSTEMSTATE_NONE			= 0b0000,
	SYSTEMSTATE_ON	 			= 0b0001,
	SYSTEMSTATE_CONNECTED		= 0b0010,
	SYSTEMSTATE_OFF 			= 0b0100,
	SYSTEMSTATE_MAX
};

enum LED{
    LED_R = 0,
    LED_G,
    LED_MAX
};

enum LEDFunc{
    LED_OFF = 0,
    LED_ON,
    LED_BLINK,
    LED_FUNMAX
};

typedef enum{
    LED_500MSEC_ONOFF,
    LED_1000MSEC_ONOFF
}LEDBlinkMenu;

typedef enum{
    Btn_None = 0,
    Btn_1 ,            // select Red LED toggle
    Btn_2,                // select Green LED blink
    Btn_3,                // Reset all LED state
    Btn_max
}KeyPad;

typedef enum{
    PRESS = 0,
    RELEASE,
    MAXSTATE
}PinState;


void setTimer(int millisec);
void toggleRedLed(void);
void blinkGreenLed(void);
void resetLedState(void);
void pushButton(int inputkey, void *tmrcallback(int inputkey));
void translateKey(int input);
void ledApp(void);

int ledState[LED_MAX];
KeyPad keyId;

void setTimer(int millisec)
{
    struct timeval tv;
  time_t t;
  time_t expTimer;
  struct tm *info;
  char buffer[64];
  int timeDifference = 0;
  
  gettimeofday(&tv, NULL);
  t = tv.tv_sec;
  timeDifference = (t*1000) + (millisec) - 0;
//  printf("exp time in millisecond: %d", t);
//  printf("\n");
  while(timeDifference > 0)
  {
      gettimeofday(&tv, NULL);
      t = tv.tv_sec;
      timeDifference -= t;
 //     printf("timer up in second: %d", t);
 //     printf("\n");
  }
}

void toggleRedLed(void)
{
    printf("Toggle Red: state = %d\n", ledState[LED_R]);
    if(ledState[LED_R] == LED_OFF)
    {
        printf("Red LED ON");
        printf("\n");
        ledState[LED_R] = LED_ON;
        setTimer(1000);
    }
    else
    {
        printf("Red LED OFF");
        printf("\n");
        ledState[LED_R] = LED_OFF;
        setTimer(1000);
    }
}

void blinkGreenLed(void)
{
    ledState[LED_G] = LED_BLINK;
    printf("Blink green: state = %d\n", ledState[LED_G]);
    
    if(ledBlinkMenu == LED_500MSEC_ONOFF)
    {
        ledBlinkMenu = LED_1000MSEC_ONOFF;
         printf("Green LED ON");
         ledState[LED_G] = LED_ON;
         printf("\n");
         setTimer(1000);
         printf("Green LED OFF");
         ledState[LED_G] = LED_OFF;
         printf("\n");
         setTimer(1000);
         
    }
    else
    {
        ledBlinkMenu = LED_500MSEC_ONOFF;
        printf("Green LED ON");
         ledState[LED_G] = LED_ON;
         printf("\n");
         setTimer(500);
         printf("Green LED OFF");
         ledState[LED_G] = LED_OFF;
         printf("\n");
         setTimer(500);
    }
}

void resetLedState(void)
{
    ledState[LED_R] = LED_OFF;
    ledState[LED_G] = LED_OFF;
    printf("All LED Off");
    printf("\n");
}

void ledApp(void)
{
    switch(keyId)
    {
        case Btn_1:
        {
            toggleRedLed();
        }
            break;
        case Btn_2: 
        {
            blinkGreenLed();
        }
            break;
        case Btn_3:
        {
            bLedReset = true;
            resetLedState();
        }
        default:
            break;    
    }   
}
void translateKey(int input)
{
    switch(input)
    {
        case 1:
            keyId = Btn_1;
            break;
        case 2:
            keyId = Btn_2;
            break;
        case 3:
            keyId = Btn_3;
            break;
        default:
            keyId = Btn_max;
            break;
            
    }
    printf("Translate key: %d\n", keyId);
    if(keyId != Btn_max)
    {
        while(1)
        {
            ledApp();
        }
        
    }
}
void tmrcallback(int inputkey)
{
    translateKey(inputkey);
}
void pushButton(int inputkey, void *tmrcallback(int inputkey))
{
    int mask = KEYDEBOUNCEMASK(inputkey);
    setTimer(100);
    if(mask == KEYDEBOUNCEMASK(inputkey) )   /* to make sure current key is press state */
    {
        printf("Input key: %d\n", inputkey);
        tmrcallback(inputkey);

    }
}

int main() {
   int inputkey;
   int sysState = SYSTEMSTATE_ON;
   char c = 'y';
   
   if(sysState = SYSTEMSTATE_ON)
   {
       for(int i= 0; i < LED_MAX; i++)
       {
           ledState[i] = LED_OFF;
       }
      
        scanf("%d", &inputkey);
        pushButton(inputkey, tmrcallback);
  
   }
    return 0;
}