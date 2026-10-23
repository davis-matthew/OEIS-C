/*
 * A046180  Hexagonal pentagonal numbers.
 * https://oeis.org/A046180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37634*a(n-1) - a(n-2) + 3136 with a(1)=1, a(2)=40755
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)40755;

    seq_t a_prev2 = (seq_t)1;        /* a(1)   */
    seq_t a_prev1 = (seq_t)40755;    /* a(2)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)37634 * a_prev1 - a_prev2 + (seq_t)3136;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
