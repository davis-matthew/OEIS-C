/*
 * A238831  a(n) = 0 if n <= 2; thereafter a(n) = A238827(n) + A238830(n-2).
 * https://oeis.org/A238831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=0 = A238827(n)+A238830(n‑2) for n>2
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 30, u64 n <= 54, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from Q(x)= (x+1)^2*(x^7-3x^6-x^5-x^4+4x^3-3x+1):
   a(n)=a(n-1)+5·a(n-2)−a(n-3)−7·a(n-4)−a(n-5)+6·a(n-6)+6·a(n-7)+a(n-8)−a(n-9)+p(n)
   where p(n)=1 (n=6), 2 (n=7), −1 (n=9), −2 (n=10), 0 otherwise.   */

#include <stddef.h>

static const seq_t init[9] = {
    (seq_t)0, /* a(1) */
    (seq_t)0, /* a(2) */
    (seq_t)0, /* a(3) */
    (seq_t)0, /* a(4) */
    (seq_t)0, /* a(5) */
    (seq_t)1, /* a(6) */
    (seq_t)3, /* a(7) */
    (seq_t)8, /* a(8) */
    (seq_t)21 /* a(9) */
};

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n <= 9)
        return init[n - 1];

    /* last[0] … last[8] will hold a(n-9)…a(n-1) throughout the loop */
    seq_t last[9];
    for (int i = 0; i < 9; ++i)
        last[i] = init[i];

    seq_t term;
    for (idx_t i = 10; i <= n; ++i)
    {
        term = last[8]                                 /* a(i-1) */
             + (seq_t)5 * last[7]                      /* 5·a(i-2) */
             - last[6]                                 /* −a(i-3) */
             - (seq_t)7 * last[5]                      /* −7·a(i-4) */
             - last[4]                                 /* −a(i-5) */
             + (seq_t)6 * last[3]                      /* +6·a(i-6) */
             + (seq_t)6 * last[2]                      /* +6·a(i-7) */
             + last[1]                                 /* +a(i-8) */
             - last[0];                                /* −a(i-9) */

        /* p(i) contributes only for i = 10 */
        if (i == 10)
            term -= (seq_t)2;                          /* p(10) = -2 */

        /* Shift window: discard a(i-9) and append new term */
        for (int k = 0; k < 8; ++k)
            last[k] = last[k + 1];
        last[8] = term;
    }

    return last[8];  /* a(n) after loop ends */
}
