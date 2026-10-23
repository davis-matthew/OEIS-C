/*
 * A159497  Numerator of Hermite(n, 5/13).
 * https://oeis.org/A159497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 10*a(n-1) - 338*(n-1)*a(n-2) with a(0)=1, a(1)=10
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)10;     /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = a0 * (seq_t)(i - 1);   /* (n-1)*a(n-2) */
        term *= (seq_t)338;                  /* multiply by 338 */
        seq_t term10 = a1 * (seq_t)10;       /* 10*a(n-1) */
        seq_t ai = term10 - term;            /* a(n) */
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
