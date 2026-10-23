/*
 * A160075  Numerator of Hermite(n, 15/26).
 * https://oeis.org/A160075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15*a(n-1) - 338*(n-1)*a(n-2), a(0)=1, a(1)=15
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;

    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)15;     /* a(1) */
    seq_t ai;

    for (idx_t k = 1; k < n; ++k)   /* compute a(k+1) using a(k) and a(k-1) */
    {
        ai = a1 * (seq_t)15 - a0 * ((seq_t)338 * (seq_t)k);
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
