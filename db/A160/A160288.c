/*
 * A160288  Numerator of Hermite(n, 26/29).
 * https://oeis.org/A160288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 26/29) Recurrence: a(0)=1, a(1)=52,
 *            a(n+1)=52*a(n)-1682*n*a(n-1) (for n >= 1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 52;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 52;  /* a(1) */
    seq_t curr;

    for (idx_t k = 1; k < n; ++k) {
        /* compute a(k+1) = 52*a(k) - 1682*k*a(k-1) */
        curr = 52 * prev1 - (seq_t)1682 * (seq_t)k * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;  /* prev1 holds a(n) after the loop */
}
