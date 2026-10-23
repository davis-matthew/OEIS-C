/*
 * A159017  Numerator of Hermite(n, 3/8).
 * https://oeis.org/A159017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 3*a(n-1) - 32*(n-1)*a(n-2), with a(0)=1, a(1)=3
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 16, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n)=4^n*Hermite(n,3/8) via the recurrence derived from
   the Hermite polynomial recurrence. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a_prev2 = (seq_t)1;  /* a(0) */
    seq_t a_prev1 = (seq_t)3;  /* a(1) */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t term = (seq_t)32 * (seq_t)(k - 1);   /* 32*(k-1) */
        seq_t cur   = (seq_t)3 * a_prev1 - term * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
