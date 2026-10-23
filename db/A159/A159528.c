/*
 * A159528  Numerator of Hermite(n, 15/16).
 * https://oeis.org/A159528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 8^n * Hermite(n, 15/16)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 15,
   a(n) = 15·a(n-1) – 128·(n‑1)·a(n-2)   (n ≥ 2) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;

    /* iterate the recurrence */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)15;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = (seq_t)128 * (seq_t)(i - 1) * a_prev2;
        a_curr = (seq_t)15 * a_prev1 - term;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
