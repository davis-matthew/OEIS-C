/*
 * A160067  Numerator of Hermite(n, 23/25).
 * https://oeis.org/A160067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n*Hermite(n,23/25) Recurrence: a(0)=1, a(1)=46, a(n+1)
 *            = 46*a(n) - 1250*n*a(n-1) (for n ≥ 1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)46;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)46;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 1; i < n; ++i) {  /* i = index of a_prev1 in the recurrence */
        /* term = 1250 * i * a_prev2 */
        seq_t term = (seq_t)i * a_prev2;
        term = (seq_t)1250 * term;

        a_curr = (seq_t)46 * a_prev1 - term;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
