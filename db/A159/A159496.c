/*
 * A159496  Numerator of Hermite(n, 4/13).
 * https://oeis.org/A159496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n,4/13)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8,
   a(n) = 8*a(n-1) - 338*(n-1)*a(n-2)   for n>=2 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur   = 0;

    /* Iterate up to n */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute cur = 8*prev1 - 338*(i-1)*prev2 */
        cur = (seq_t)8 * prev1 - (seq_t)338 * (i - 1) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
