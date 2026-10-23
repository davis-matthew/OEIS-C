/*
 * A159500  Numerator of Hermite(n, 7/13).
 * https://oeis.org/A159500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n,7/13)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=14, a(n+1)=14*a(n)-338*n*a(n-1) for n≥1 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;

    /* Iterative computation using the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)14;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* A_i = 14*A_{i-1} - 338*(i-1)*A_{i-2} */
        seq_t mult = (seq_t)338 * (seq_t)(i - 1);
        curr = (seq_t)14 * prev1 - mult * prev2;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
