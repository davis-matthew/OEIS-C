/*
 * A127190  Expansion of e.g.f. sqrt((1+2*x)/(1+4*x)).
 * https://oeis.org/A127190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(1) = -1; for n ≥ 2, a(n) + (6*n-5)*a(n-1) +
 *            8*(n-1)*(n-2)*a(n-2) = 0 (see A127190 – triangular recurrence
 *            for sqrt((1+2*x)/(1+4*x)))
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;

    seq_t a_prev2 = (seq_t)1;          /* a(0) */
    seq_t a_prev1 = (seq_t)-1;         /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = (seq_t)(6 * (seq_t)i - 5) * a_prev1;
        seq_t term2 = (seq_t)8 * (seq_t)(i - 1) * (seq_t)(i - 2) * a_prev2;
        a_curr = -(term1 + term2);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
