/*
 * A226701  Positive solutions x/(3*13) of the Pell equation x^2 - 61*y^2 =
 *          -4.
 * https://oeis.org/A226701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1523*a(n-1) - a(n-2), a(-1) = -1, a(0) = 1, a(1) = 1524
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1524;
    seq_t a_prev2 = (seq_t)1;          /* a(0) */
    seq_t a_prev1 = (seq_t)1524;        /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_cur = (seq_t)1523 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_prev1;
}
