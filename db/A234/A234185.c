/*
 * A234185  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) + 120*a(n-2) - 576*a(n-3) - 3026*a(n-4)
                 + 5484*a(n-5) + 6720*a(n-6) - 10080*a(n-7),
   with initial values a(1)=412, a(2)=3710, a(3)=33720,
   a(4)=307598, a(5)=2802904, a(6)=25588052, a(7)=233283984. */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* Initial values for 1..7 */
    static const seq_t init[8] = {
        (seq_t)0,            /* unused, a(0) */
        (seq_t)412,          /* a(1) */
        (seq_t)3710,         /* a(2) */
        (seq_t)33720,        /* a(3) */
        (seq_t)307598,       /* a(4) */
        (seq_t)2802904,      /* a(5) */
        (seq_t)25588052,     /* a(6) */
        (seq_t)233283984     /* a(7) */
    };

    if (n <= 7)
        return init[n];

    /* Window of the last seven terms */
    seq_t prev1 = init[7];
    seq_t prev2 = init[6];
    seq_t prev3 = init[5];
    seq_t prev4 = init[4];
    seq_t prev5 = init[3];
    seq_t prev6 = init[2];
    seq_t prev7 = init[1];
    seq_t cur = (seq_t)0;

    /* Iterate from 8 up to n */
    for (idx_t i = 8; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = mul_small(prev1, 6);
        pos += mul_small(prev2, 120);
        pos += mul_small(prev5, 5484);
        pos += mul_small(prev6, 6720);

        /* Negative contributions */
        seq_t neg = mul_small(prev3, 576);
        neg += mul_small(prev4, 3026);
        neg += mul_small(prev7, 10080);

        /* Next term */
        cur = pos;
        cur -= neg;

        /* Shift window */
        prev7 = prev6;
        prev6 = prev5;
        prev5 = prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
