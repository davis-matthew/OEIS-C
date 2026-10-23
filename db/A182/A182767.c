/*
 * A182767  Beatty sequence for 1+e^2.
 * https://oeis.org/A182767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1+e^2)) = floor(n + n*e^2)
 * Range:     u8 n <= 30, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes floor(n*(1+e^2)) for n >= 1 */
seq_t a(idx_t n)
{
    /* Use an extended precision type for intermediate calculations.
       The maximum factorial needed is 30! which fits in 128 bits. */
    unsigned __int128 n128   = n;   /* n as 128-bit integer */
    unsigned __int128 Q      = n128;   /* integer part of the partial sum */
    unsigned __int128 R      = 0;      /* remainder numerator (numerator of fractional part) */
    unsigned __int128 fact   = 1;      /* (k-1)! for the current step, start with 0! = 1 */

    /* Predefined number of terms sufficient to guarantee the tail < 1
       for all 64‑bit n (K=30). */
    const int K = 30;
    for (int k = 1; k <= K; ++k) {
        unsigned __int128 pow2 = (unsigned __int128)1 << k;      /* 2^k */
        unsigned __int128 numerator = R * k + n128 * pow2;       /* R*k + n*2^k */

        fact *= k;                                           /* fact = k! */

        unsigned __int128 add = numerator / fact;            /* integer part contributed by this term */
        Q += add;                                            /* update sum's integer part */

        R = numerator % fact;                                /* fractional remainder for next step */
    }

    /* floor(n*e^2) is now Q. The sequence value is n + Q. */
    unsigned __int128 result128 = n128 + Q;
    /* Cast back to seq_t; if seq_t is too small the overflow is allowed by the specs. */
    return (seq_t)result128;
}
