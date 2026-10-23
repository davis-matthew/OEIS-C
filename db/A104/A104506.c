/*
 * A104506  Column 1 of triangle A104505, which is equal to the right-hand
 *          side of the triangle A084610 of coefficients in (1 + x - x^2)^n.
 * https://oeis.org/A104506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ( n*(2n-1)*a(n-1) - 5*n*(n-1)*a(n-2) ) / ( (n-1)*(n+1) ),
 *            with a(0) = 0, a(1) = -1. This uses the linear recurrence from
 *            the OEIS entry and evaluates it iteratively, so it works for any
 *            seq_t that supports +, -, *, / and assignment. No dynamic
 *            allocation is required
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 21, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)-1;

    seq_t a_prev2 = (seq_t)0;          /* a(0) */
    seq_t a_prev1 = (seq_t)-1;         /* a(1) */
    seq_t a_curr = a_prev1;            /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_seq   = (seq_t)i;
        seq_t term1   = i_seq * ((seq_t)2 * i_seq - (seq_t)1) * a_prev1;
        seq_t term2   = (seq_t)5 * i_seq * (i_seq - (seq_t)1) * a_prev2;
        seq_t numer   = term1 - term2;
        seq_t denom   = ((seq_t)(i - 1)) * ((seq_t)(i + 1));
        a_curr = numer / denom;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
