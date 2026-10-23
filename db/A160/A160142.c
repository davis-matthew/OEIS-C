/*
 * A160142  Numerator of Hermite(n, 16/27).
 * https://oeis.org/A160142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=32, a(n) = 32*a(n-1) - 1458*(n-1)*a(n-2) for n>=2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from Hermite(n,16/27) with a(n)=27^n*Hermite(n,16/27) */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }
    if (n == 1u) {
        return (seq_t)32;
    }
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)32;  /* a(1) */
    seq_t cur = 0;
    for (idx_t k = 2; k <= n; ++k) {
        /* term = 1458*(k-1)*a_prev2 */
        seq_t term = (seq_t)1458 * ((seq_t)(k - 1)) * a_prev2;
        cur = (seq_t)32 * a_prev1 - term;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return cur;
}
