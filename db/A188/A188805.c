/*
 * A188805  The n-th derivative of 1/(1-x-x^2), evaluated at x=1.
 * https://oeis.org/A188805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*(n-1)*a(n-2) + 3*n*a(n-1) + a(n) = 0,
   initial values: a(0) = -1, a(1) = 3 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)-1;
    if (n == 1)
        return (seq_t)3;

    /* a_{i-2} and a_{i-1} will hold successive values */
    seq_t a_prev2 = (seq_t)-1;   /* a(0) */
    seq_t a_prev1 = (seq_t)3;    /* a(1) */
    seq_t a_curr  = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        /* Compute a(i) = -(i*(i-1)*a(i-2) + 3*i*a(i-1)) */
        a_curr = -(ii * (ii - 1) * a_prev2 + (seq_t)3 * ii * a_prev1);

        /* Shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
