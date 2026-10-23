/*
 * A159019  Numerator of Hermite(n, 5/8).
 * https://oeis.org/A159019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=5 a(n) = 5*a(n-1) - 32*(n-1)*a(n-2) for n >= 2
 *            (OEIS A159019)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 16, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)5;   /* a(n-1) */
    seq_t a2;              /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* term = 32 * (i-1) * a(n-2) */
        seq_t term = a0 * (seq_t)(i - 1);   /* multiply by (i-1) */
        term *= (seq_t)32;                  /* multiply by 32 */

        /* a(n) = 5*a(n-1) - term */
        a2 = (seq_t)5 * a1;
        a2 -= term;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
