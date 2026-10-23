/*
 * A159859  Numerator of Hermite(n, 2/23).
 * https://oeis.org/A159859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 23^n * Hermite(n, 2/23)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from Hermite polynomial:
   a(0)=1, a(1)=4,
   a(n)=4*a(n-1)-1058*(n-1)*a(n-2) for n≥2. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)4;   /* a(1) */
    seq_t term;
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 1058*(i-1)*a(i-2) */
        term = prev1 * ((seq_t)4) - prev2 * ((seq_t)1058) * ((seq_t)(i - 1));
        prev2 = prev1;
        prev1 = term;
    }
    return prev1;
}
