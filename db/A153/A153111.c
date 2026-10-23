/*
 * A153111  Solutions of the Pell-like equation 1 + 6*A*A = 7*B*B, with A, B
 *          integers.
 * https://oeis.org/A153111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - a(n-2), a(1)=1, a(2)=25
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

extern seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)25;

    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)25;  /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)26 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
