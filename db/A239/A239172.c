/*
 * A239172  Number of (n+1)X(2+1) 0..2 arrays with no element greater than all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A239172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for OEIS A239172:
   a(n) = 15*a(n-2) +149*a(n-3) +319*a(n-4) +372*a(n-5) -742*a(n-6) 
          -1302*a(n-7) +296*a(n-8) +279*a(n-9) +1010*a(n-10) -921*a(n-11) 
          +278*a(n-12) +747*a(n-13) -508*a(n-14) +361*a(n-15) -662*a(n-16) 
          +626*a(n-17) -360*a(n-18) +80*a(n-19)
   with initial values
   a(1)=26, a(2)=82, a(3)=676, a(4)=4338, a(5)=29380, a(6)=196698,
   a(7)=1321986, a(8)=8867938, a(9)=59528520, a(10)=399598612,
   a(11)=2681935810, a(12)=18001714474, a(13)=120831198520,
   a(14)=811019710990, a(15)=5443667518288, a(16)=36538530014326,
   a(17)=245249725777154, a(18)=1646142454877826, a(19)=11049078946194810
*/

#include <stddef.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* initial 19 terms */
    static const seq_t init[19] = {
        26, 82, 676, 4338, 29380, 196698, 1321986,
        8867938, 59528520, 399598612, 2681935810,
        18001714474, 120831198520, 811019710990,
        5443667518288, 36538530014326, 245249725777154,
        1646142454877826, 11049078946194810
    };

    /* coefficients for a(n-2) .. a(n-19) */
    static const int coeff[19] = {
        15, 149, 319, 372, -742, -1302, 296, 279, 1010,
        -921, 278, 747, -508, 361, -662, 626, -360, 80
    };

    if (n <= 19) {
        return init[n - 1];
    }

    /* buffer holding the last 19 computed terms:
       buf[0] = a(n-19), buf[18] = a(n-1) at each iteration   */
    seq_t buf[19];
    for (int i = 0; i < 19; ++i) {
        buf[i] = init[i];
    }

    /* compute terms 20..n */
    for (idx_t term = 20; term <= n; ++term) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* evaluate the recurrence */
        for (int k = 0; k < 19; ++k) {
            int m = k + 2;                     /* offset: a(term-m) */
            seq_t val = buf[19 - m];           /* buffer index for a(term-m) */

            if (coeff[k] > 0) {
                pos += mul_small(val, coeff[k]);
            } else {
                neg += mul_small(val, (unsigned int)(-coeff[k]));
            }
        }

        seq_t new_val = pos - neg;              /* final term value */

        /* shift buffer left by one, insert new_val at end */
        for (int j = 0; j < 18; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[18] = new_val;
    }

    return buf[18];  /* a(n) */
}
