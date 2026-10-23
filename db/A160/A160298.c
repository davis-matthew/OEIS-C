/*
 * A160298  Numerator of Hermite(n, 29/30).
 * https://oeis.org/A160298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29 a(n-1) - 450 (n-1) a(n-2) a(0) = 1, a(1) = 29 This
 *            follows from the recurrence of Hermite polynomials H_{n+1}(x) =
 *            2x H_n(x) - 2n H_{n-1}(x) with x = 29/30 and a(n) = 15^n
 *            H_n(29/30)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)29;

    seq_t a_prev2 = (seq_t)1;          /* a(0) */
    seq_t a_prev1 = (seq_t)29;         /* a(1) */
    seq_t a_cur   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 29 a(i-1) - 450 (i-1) a(i-2) */
        a_cur = (seq_t)29 * a_prev1
                - (seq_t)(450 * (i - 1)) * a_prev2;

        /* Roll variables for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}
