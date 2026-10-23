/*
 * A160336  Numerator of Hermite(n, 25/31).
 * https://oeis.org/A160336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of Hermite(n, 25/31) Recurrence: a(0) = 1, a(1)
 *            = 50, a(n+1) = 50*a(n) - 2*n*961*a(n-1) (for n ≥ 1) where the
 *            denominator of a(n) is 31^n and thus the numerator satisfies the
 *            above integer recurrence
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)50;

    seq_t a_prev = (seq_t)1;      /* a(0) */
    seq_t a_curr = (seq_t)50;     /* a(1) */
    for (idx_t k = 1; k < n; ++k) {
        /* compute a(k+1) using the recurrence */
        seq_t term = (seq_t)2 * (seq_t)k * (seq_t)961 * a_prev;
        seq_t a_next = (seq_t)50 * a_curr - term;
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
