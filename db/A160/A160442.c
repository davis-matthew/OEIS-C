/*
 * A160442  Numerator of Hermite(n, 29/32).
 * https://oeis.org/A160442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 29, a(n) = 29 * a(n-1) - 512 * (n-1) * a(n-2)
 *            for n >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"

/* External linkage function required by the build */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return seq_one();
    if (n == 1) return (seq_t)29;

    /* Iterate from 2 up to n, keeping only the last two values */
    seq_t a_prev2 = seq_one();          /* a(0)  */
    seq_t a_prev1 = (seq_t)29;          /* a(1)  */
    seq_t a_cur   = 0;                  /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 29*a(i-1) - 512*(i-1)*a(i-2) */
        a_cur = (seq_t)29 * a_prev1
                - (seq_t)512 * (seq_t)(i - 1) * a_prev2;

        /* Shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}
