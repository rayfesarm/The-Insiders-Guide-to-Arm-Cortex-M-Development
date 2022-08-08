/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_power.h"
#include <stdio.h>
#include "arm_math.h"


#define force_inline always_inline

#define float32_t float
#define MAX_BLOCKSIZE    256
#define DELTA           (0.000001f)

float32_t srcA[MAX_BLOCKSIZE] = 
{
    4.458926, -2.645590, 4.191267, -3.074756, 
    -0.733985, 4.034804, 4.016267, -0.432540, 
    -0.328012, -1.921692, -0.202806, 4.871618, 
    3.050932, -0.200686, -1.557964, 1.305450, 
    -3.843157, 1.266553, -2.985137, 4.163619, 
    0.981371, 4.660072, 4.190737, -0.600573, 
    0.835867, 4.614466, 3.775712, 2.327756, 
    1.961031, -1.791719, 1.006438, 3.651250, 
    0.571409, 3.183929, 4.024144, -4.536352, 
    -2.377524, -3.127108, -1.647092, -3.703881, 
    3.511522, 2.965792, -1.086886, 0.738509, 
    -2.141485, -0.942030, -3.712234, 3.286732, 
    -2.984296, 1.658533, -3.321015, 4.497620, 
    3.384757, -4.561316, 3.453638, -1.200756, 
    1.341035, 2.820187, 4.641099, -1.975492, 
    2.407484, -4.473910, 2.259541, 1.330246, 
    0.314499, 0.798059, 0.781184, 3.507867, 
    2.899511, -2.817744, -3.302779, 3.913036, 
    -0.070815, 0.358719, -0.650890, -2.155626, 
    -2.506923, -0.377420, 2.268402, -0.851335, 
    0.409196, -4.969564, -3.724370, 0.202533, 
    -4.733618, 3.801633, 4.442995, -0.434061, 
    -4.645307, -1.384677, 0.977772, -2.767673, 
    1.760948, 3.796296, -0.394925, -0.199526, 
    0.358896, 2.292904, 3.813321, 4.161705, 
    3.360366, 4.774492, -3.685096, 0.960790, 
    -2.558600, 2.022042, -0.732247, 1.883512, 
    1.546858, 0.835466, 4.445011, 4.771632, 
    2.915793, -4.841482, 1.377746, 0.195776, 
    0.967338, -0.472792, -3.846267, -2.589227, 
    4.491143, -0.353175, 3.932809, -2.132006, 
    -1.911685, -2.996864, -3.090855, -3.929923, 
    4.857299, 4.726572, -4.936661, -3.968035, 
    3.571346, -3.804603, -2.695413, 3.519955, 
    -0.694597, 4.025900, -1.186955, 0.187484, 
    2.493056, -1.855756, -3.297234, 1.027609, 
    -1.061355, 3.653752, -2.789185, -2.007228, 
    -1.579429, -2.961235, 3.867461, 0.774523, 
    3.474638, -2.689560, 1.249273, 4.793627, 
    -0.466313, -2.590214, 2.621241, -1.690694, 
    0.359347, -2.735806, 2.997870, 4.317170, 
    0.463848, -4.748850, 2.639036, 2.300272, 
    0.044418, 0.477243, 2.769575, -1.104981, 
    -0.085102, 1.423800, 4.030082, -2.423171, 
    -4.056124, 2.456444, 0.999755, 3.339286, 
    0.805608, -3.767237, -4.779595, -0.892559, 
    -2.011682, 3.464155, -3.604109, 4.141224, 
    -4.077085, 1.927678, -0.248017, 1.216039, 
    -2.351529, -0.057495, -2.875939, -0.886774, 
    4.353178, -4.068374, -3.333084, -1.647416, 
    3.667364, 2.275477, -0.914834, 2.719882, 
    -4.315918, 2.954308, -3.756594, -4.744190, 
    0.556621, 1.861905, 4.800767, 2.133157, 
    -3.542010, -0.095746, -4.247009, -3.514515, 
    0.274678, 2.821656, 1.333616, -1.667755, 
    -1.773252, -2.475500, -4.654981, 1.101859, 
    2.045102, 2.549487, 0.098796, -2.144779, 
    -4.102854, -2.525800, -3.150636, -4.838069, 
    -3.830190, 4.386980, 2.412821, 2.069383, 
    -2.910054, -2.501668, 4.029315, -2.657239, 
    2.962891, -2.671260, -4.448526, -1.052162, 
    -1.244414, -2.164066, -2.033339, 3.719992, 
    -0.058085, -1.566909, 2.550844, 1.164383, 
    -3.597650, -2.827309, -1.720990, 1.433874, 
};


float32_t srcB[MAX_BLOCKSIZE] = 
{
    4.717317, 4.590384, 1.371975, -0.995061,
    -4.271574, -0.958936, -4.321522, 1.349417,
    4.454786, 4.021796, -3.512528, -1.082006,
    4.811365, -1.763877, -4.093163, -1.602019,
    -4.224507, 1.266543, -0.048028, 3.862496,
    2.239611, -0.363927, -1.052136, 4.514933,
    4.815421, -2.663236, -3.980508, 3.552231,
    0.577191, 3.509286, -0.988025, -1.491155,
    0.107373, 3.172332, 0.955536, 3.961783,
    3.633199, -3.454275, -1.215317, 0.964997,
    -1.026780, 3.882326, -3.030864, 3.563354,
    -4.568335, 2.396918, -3.396111, -1.707234,
    1.801327, -3.207259, 1.877808, 3.994864,
    4.473581, -4.077531, 4.252438, 0.552332,
    -3.073291, -3.369170, 2.527082, -0.802074,
    -3.134735, -4.302323, 0.793880, -4.816259,
    -3.114160, 2.439411, 2.586680, 1.722307,
    -3.806216, -2.714294, -4.730696, 4.845974,
    -4.459527, -3.754100, -4.495730, -2.311012,
    2.943035, 2.934908, 4.416888, -1.759251,
    3.467076, -2.660830, -0.676749, -3.523812,
    -1.042693, -0.878715, 0.503771, -3.890428,
    -1.320196, -2.251898, 2.994960, 4.920164,
    0.114545, -3.197641, 2.737956, 3.035960,
    -4.537219, -1.043371, 3.891836, -4.652659,
    4.998758, 3.274063, -1.849806, 0.342652,
    -0.347313, -3.798292, 4.214490, 3.415075,
    -1.014131, -2.803982, 3.942473, -2.301012,
    -1.425941, 4.085765, -2.781762, 3.091166,
    3.008275, 1.544846, 2.378178, 3.629506,
    2.673047, 0.907184, -0.169625, -2.078179,
    2.982247, -2.291950, 4.965417, 2.401525,
    -0.588381, -0.680964, 4.229613, -0.196645,
    -0.611945, 2.409435, 0.600778, -3.977240,
    1.171837, 2.403158, -2.988046, 1.980411,
    4.464107, 3.071798, 2.114689, 2.589621,
    2.509632, 4.176268, 3.236258, 1.741229,
    4.371719, 4.869287, 1.163358, 2.063249,
    -1.748764, 0.781372, -2.703790, 4.121678,
    -4.411772, 3.916374, -1.936632, -1.338424,
    -0.907289, 4.260266, -1.591172, 0.436326,
    1.386693, 2.083749, 1.477058, 4.896578,
    1.326703, 2.553598, 4.532769, -4.906132,
    0.368240, -2.160820, 1.890424, 2.243543,
    -4.347137, -2.372076, 3.918369, 0.486212,
    0.961960, 2.348978, 0.292536, -4.723923,
    -2.384478, 1.393335, -3.404875, 3.880198,
    -2.873096, -4.052622, -2.046560, -0.541910,
    -1.871569, -2.762439, -4.453276, -2.665497,
    2.355677, 0.652585, 0.485372, 3.583903,
    -3.651689, 4.237660, -4.429107, 4.320596,
    1.826152, -1.077139, 4.532059, 0.241892,
    -0.069611, 3.686567, 3.386922, -0.896227,
    0.550064, -3.563355, 3.484654, -4.463936,
    0.150321, -1.759696, 1.157667, -2.236653,
    -0.849533, -3.040854, 4.467019, 2.099517,
    4.972395, -3.679843, -4.754458, -1.344235,
    -4.560917, 0.122001, 1.066238, 0.641708,
    -0.857938, -0.183194, -3.402014, 2.416509,
    2.798100, 4.377564, -4.689146, 4.191264,
    3.085867, -0.698762, -3.928355, 3.681785,
    -4.309487, 0.042974, 2.124313, 3.963325,
    -4.995780, 1.121975, 4.426868, 2.078657,
    -2.778396, 0.856749, -1.833541, -1.266217,
};


float32_t multOutput[MAX_BLOCKSIZE];  /* Intermediate output */
float32_t testOutput;  /* Final ouput */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* DWT Variables */
#define CM_DWT_CYCCNTENA_BIT   (1UL<<0)
#define CM_TRCENA_BIT          (1UL<<24)

#define CM_DWT_CONTROL         (*((volatile uint32_t*)0xE0001000))
#define CM_DWT_CYCCNT          (*((volatile uint32_t*)0xE0001004))
#define CM_DWT_LAR             (*((volatile uint32_t*)0xE0001FB0))
#define CM_DEMCR               (*((volatile uint32_t*)0xE000EDFC))

#define CM_DWT_CPICNT          (*((volatile uint32_t*)0xE0001008))
#define CM_DWT_EXCCNT          (*((volatile uint32_t*)0xE000100C))
#define CM_DWT_SLEEPCNT        (*((volatile uint32_t*)0xE0001010))
#define CM_DWT_LSUCNT          (*((volatile uint32_t*)0xE0001014))
#define CM_DWT_FOLDCNT         (*((volatile uint32_t*)0xE0001018))



/*******************************************************************************
 * Code
 ******************************************************************************/

void start_dwt()
{
    CM_DEMCR |= CM_TRCENA_BIT;
    CM_DWT_LAR = 0xC5ACCE55;
    CM_DWT_CYCCNT = 0;
    CM_DWT_CONTROL |= CM_DWT_CYCCNTENA_BIT; 
}
void stop_dwt()
{
    CM_DWT_CONTROL &= ~CM_DWT_CYCCNTENA_BIT;
	  PRINTF("CCNT = %u\n", CM_DWT_CYCCNT);
}

float dot_product(float v1[], float v2[], int length);
float dot_product2(float v1[], float v2[], int length);
float dot_product3(float v1[], float v2[], int length);

float dot_product3(float v1[], float v2[], int length)
{

    float result;

    (void) start_dwt();

    (void) arm_dot_prod_f32(v1, v2, length, &result);

    (void) stop_dwt();

    return result;

}

float dot_product2(float v1[], float v2[], int length)
{
    uint32_t i;         /* Loop counter */
    float32_t diff;     /* Difference between reference and test outputs */

    (void) start_dwt();

    /* Multiplication of two input buffers */
    arm_mult_f32(v1, v2, multOutput, MAX_BLOCKSIZE);

    /* Accumulate the multiplication output values to
       get the dot product of the two inputs */
    for(i=0; i< MAX_BLOCKSIZE; i++)
    {
        arm_add_f32(&testOutput, &multOutput[i], &testOutput, 1);
    }

    (void) stop_dwt();

  return testOutput ;
}


// accepts two vectors (arrays) as arguments and a length, computes and returns
// the dot product
float dot_product(float v1[], float v2[], int length)
{
    // we compute the dot product by multiply the correspdoning elements in each 
    // array and adding them to a sum
    float sum = 0;

    (void) start_dwt();

    for (int i = 0; i < length; i++)
      sum += v1[i] * v2[i];

    (void) stop_dwt();

    return sum;
}

int main() 
{
	 char ch;


    /* Init board hardware. */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    float32_t result = dot_product(srcA, srcB, MAX_BLOCKSIZE);
    PRINTF("Dot product function complete\n");

    float32_t result2 = dot_product2(srcA, srcB, MAX_BLOCKSIZE);
    PRINTF("Dot product2 function complete\n");

    float32_t result3 = dot_product3(srcA, srcB, MAX_BLOCKSIZE);
    PRINTF("Dot product3 function complete\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
 
    return 0;
}
