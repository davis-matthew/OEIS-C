/*
 * A160263  Numerator of Hermite(n, 14/29).
 * https://oeis.org/A160263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 14/29)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=28, a(n+1)=28*a(n) - 1682*n*a(n-1) for n>=1 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 28;

    seq_t a_prev2 = 1;        /* a(0) */
    seq_t a_prev1 = 28;       /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        /* a_next = 28*a_prev1 - 1682*i*a_prev2 */
        seq_t term1 = (seq_t)28 * a_prev1;
        seq_t term2 = (seq_t)1682 * (seq_t)i * a_prev2;
        seq_t a_next = term1 - term2;
        a_prev2 = a_prev1;
        a_prev1 = a_next;
    }
    return a_prev1;
}
