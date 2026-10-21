/*
 * A221375  Number of n X 3 arrays of occupancy after each element stays put
 *          or moves to some horizontal, vertical or antidiagonal neighbor,
 *          with no occupancy greater than 2.
 * https://oeis.org/A221375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A221375
   Number of n×3 arrays of occupancy after each element stays
   put or moves to some horizontal, vertical or antidiagonal
   neighbor, with no occupancy greater than 2.
   Initial values:
     a(1)=7, a(2)=139, a(3)=3017, a(4)=68006, a(5)=1548941,
     a(6)=35365731, a(7)=807887304, a(8)=18456843366,
     a(9)=421665253450, a(10)=9633337841423,
     a(11)=220081885496712, a(12)=5027948655020101,
     a(13)=114867396786410407,
     a(14)=2624232861115617998,
     a(15)=59952562195425809756.
   For n > 15 the sequence satisfies the 14‑term linear recurrence
   (all coefficients are integers):
     a(n) =
         +  53 * a(n-1)
         - 1022 * a(n-2)
         + 9430 * a(n-3)
         - 46832 * a(n-4)
         +127474 * a(n-5)
         -173901 * a(n-6)
         + 70397 * a(n-7)
         + 60137 * a(n-8)
         - 23731 * a(n-9)
         - 21147 * a(n-10)
         -  4513 * a(n-11)
         -    40 * a(n-12)
         +  152 * a(n-13)
         +   12 * a(n-14)
*/

#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* static table of the first 15 terms, initialised on first call */
    static seq_t init[16];
    static bool ready = false;
    if (!ready) {
        init[1] = seq_from_str("7");
        init[2] = seq_from_str("139");
        init[3] = seq_from_str("3017");
        init[4] = seq_from_str("68006");
        init[5] = seq_from_str("1548941");
        init[6] = seq_from_str("35365731");
        init[7] = seq_from_str("807887304");
        init[8] = seq_from_str("18456843366");
        init[9] = seq_from_str("421665253450");
        init[10] = seq_from_str("9633337841423");
        init[11] = seq_from_str("220081885496712");
        init[12] = seq_from_str("5027948655020101");
        init[13] = seq_from_str("114867396786410407");
        init[14] = seq_from_str("2624232861115617998");
        init[15] = seq_from_str("59952562195425809756");
        ready = true;
    }

    if (n <= 15)
        return init[n];

    /* Rolling buffer containing the last 15 values:
       buf[0] = a(k-14), buf[1] = a(k-13), ... , buf[14] = a(k)
       where k is the current position under construction.  */
    static seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i + 1];        /* init[1] .. init[15] -> buf[0]..buf[14] */

    /* Compute successive terms up to n. */
    idx_t current = 15;                /* the index of the last value in buf */
    while (current < n) {
        /* Positive contributions */
        seq_t pos = 53 * buf[14]       /* a(n-1) */
                  + 9430 * buf[12]      /* a(n-3) */
                  + 127474 * buf[10]    /* a(n-5) */
                  + 70397 * buf[8]      /* a(n-7) */
                  + 60137 * buf[7]      /* a(n-8) */
                  + 152 * buf[2]        /* a(n-13) */
                  + 12 * buf[1];        /* a(n-14) */

        /* Negative contributions */
        seq_t neg = 1022 * buf[13]     /* a(n-2) */
                  + 46832 * buf[11]    /* a(n-4) */
                  + 173901 * buf[9]    /* a(n-6) */
                  + 23731 * buf[6]     /* a(n-9) */
                  + 21147 * buf[5]     /* a(n-10) */
                  + 4513 * buf[4]      /* a(n-11) */
                  + 40 * buf[3];       /* a(n-12) */

        seq_t new_val = pos - neg;     /* final recurrence value */

        /* Shift buffer to make room for new_val */
        for (int i = 0; i < 14; ++i)
            buf[i] = buf[i + 1];
        buf[14] = new_val;
        ++current;
    }

    return buf[14];  /* a(n) */
}
