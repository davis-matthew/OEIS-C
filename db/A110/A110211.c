/*
 * A110211  a(n+3) = 6*a(n) - 5*a(n+2), a(0) = -1, a(1) = 3, a(2) = -15.
 * https://oeis.org/A110211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n+3) = 6*a(n) - 5*a(n+2), a(0) = -1, a(1) = 3, a(2) = -15
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 14, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return -((seq_t)1);
    if (n == 1) return (seq_t)3;
    if (n == 2) return -( (seq_t)15 );

    seq_t a_prev3 = -((seq_t)1);
    seq_t a_prev2 = (seq_t)3;
    seq_t a_prev1 = -( (seq_t)15 );

    for (idx_t i = 3; i <= n; ++i) {
        seq_t val = 6 * a_prev3 - 5 * a_prev1;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = val;
    }
    return a_prev1;
}
