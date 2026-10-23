/*
 * A196206  Number of nX4 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 2,0,3,4,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -6*a(n-2) +7*a(n-3) -5*a(n-4) -a(n-5) -a(n-6)
 *            -6*a(n-7) +12*a(n-8) -2*a(n-9) +18*a(n-10) -29*a(n-11)
 *            +12*a(n-12) -2*a(n-13) -4*a(n-14) +16*a(n-15) -9*a(n-16)
 *            +7*a(n-17) -4*a(n-18) -3*a(n-19) for n > 19 Initial values
 *            (offset 1): a(1)=5, a(2)=11, a(3)=14, a(4)=35, a(5)=70,
 *            a(6)=136...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"

static const int coeff[19] = {
    4,  -6,   7,  -5,  -1,  -1,  -6,  12,  -2, 18,
   -29, 12,  -2,  -4,  16,  -9,   7,  -4, -3
};

static const seq_t init[20] = {
    0,          /* dummy for index 0 */
    5,          /* a(1) */
    11,         /* a(2) */
    14,         /* a(3) */
    35,         /* a(4) */
    70,         /* a(5) */
    136,        /* a(6) */
    305,        /* a(7) */
    611,        /* a(8) */
    1321,       /* a(9) */
    2832,       /* a(10) */
    6041,       /* a(11) */
    12936,      /* a(12) */
    27706,      /* a(13) */
    59515,      /* a(14) */
    128053,     /* a(15) */
    275195,     /* a(16) */
    591576,     /* a(17) */
    1272021,    /* a(18) */
    2736374     /* a(19) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n=0, return 0 */

    if (n <= 19)
        return init[n];         /* return initial value */

    /* Sliding window of the last 19 terms:
       prev[0] = a(current-1), prev[1] = a(current-2), ..., prev[18] = a(current-19)
       Initially for current = 20 we need a(19) .. a(1) */
    seq_t prev[19];
    for (int i = 0; i < 19; ++i)
        prev[i] = init[19 - i];

    seq_t result = 0;
    for (idx_t cur = 20; cur <= n; ++cur) {
        seq_t pos = 0;    /* sum of positive contributions */
        seq_t neg = 0;    /* sum of absolute values of negative contributions */

        for (int i = 0; i < 19; ++i) {
            /* magnitude of coefficient */
            unsigned mult = coeff[i] > 0 ? (unsigned)coeff[i] : (unsigned)(-coeff[i]);

            seq_t prod = mul_small(prev[i], mult);

            if (coeff[i] > 0)
                pos += prod;
            else
                neg += prod;
        }

        /* compute current term */
        result = pos - neg;      /* pos >= neg for the sequence */

        if (cur == n)
            return result;       /* final term reached */

        /* shift window: move all terms one step to the right */
        for (int i = 18; i > 0; --i)
            prev[i] = prev[i - 1];
        prev[0] = result;       /* new term becomes first in the window */
    }

    return result; /* should never reach here */
}
