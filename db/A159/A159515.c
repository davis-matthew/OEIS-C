/*
 * A159515  Numerator of Hermite(n, 4/15).
 * https://oeis.org/A159515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=8; For n≥1: a(n+1)=8*a(n) – 450*n*a(n-1) (OEIS
 *            A159515)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* idx_t */
#include <stdint.h>   /* seq_t assumed */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 8;

    seq_t a_prev2 = 1;   /* a(0) */
    seq_t a_prev1 = 8;   /* a(1) */
    seq_t an = 0;

    for (idx_t i = 1; i < n; ++i)  /* compute a(i+1) */
    {
        an = 8 * a_prev1 - (seq_t)(450 * i) * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = an;
    }
    return an;
}
