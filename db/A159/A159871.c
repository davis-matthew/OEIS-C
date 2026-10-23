/*
 * A159871  Numerator of Hermite(n, 7/23).
 * https://oeis.org/A159871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=14, a(n) = 14*a(n-1) - 1058*(n-1)*a(n-2) for n>=2
 *            where 1058 = 2*23^2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if(n == 0) return 1;
    if(n == 1) return 14;

    seq_t a0 = 1;   /* a(n-2) */
    seq_t a1 = 14;  /* a(n-1) */
    seq_t an = 0;   /* a(n) */

    for(idx_t i = 2; i <= n; ++i)
    {
        seq_t coef = 1058 * ((seq_t)(i - 1));  /* 1058 * (n-1) */
        an = 14 * a1 - coef * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
