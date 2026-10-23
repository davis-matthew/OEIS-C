/*
 * A025932  Expansion of 1/((1-2*x)*(1-3*x)*(1-5*x)*(1-8*x)).
 * https://oeis.org/A025932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 40*a(n-2) + 3^(n+1) - 2^(n+1) (n >= 2) a(0) =
 *            1, a(1) = 18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the first two values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;

    /* a(0), a(1) */
    seq_t prevprev = (seq_t)1;
    seq_t prev     = (seq_t)18;

    /* Powers: at iteration i we need 3^(i+1) and 2^(i+1).
       Start from i = 2: 3^3 and 2^3 */
    seq_t pow2 = (seq_t)8;   /* 2^(2+1) */
    seq_t pow3 = (seq_t)27;  /* 3^(2+1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 13*a(i-1) - 40*a(i-2) + 3^(i+1) - 2^(i+1) */
        seq_t cur = prev * (seq_t)13;
        cur -= prevprev * (seq_t)40;
        cur += pow3;
        cur -= pow2;

        /* Shift for next iteration */
        prevprev = prev;
        prev = cur;

        /* Update powers for next index, unless we are at the last */
        if (i < n) {
            pow2 *= (seq_t)2;
            pow3 *= (seq_t)3;
        }
    }

    return prev;   /* prev now holds a(n) */
}
