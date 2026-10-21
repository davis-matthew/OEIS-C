/*
 * A238923  Number of (n+1) X (1+1) 0..3 arrays with no element greater than
 *          all horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A238923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2), a(1) = 12, a(2) = 36
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset, harmless */

    if (n == 1)
        return (seq_t)12;
    if (n == 2)
        return (seq_t)36;

    seq_t prev2 = (seq_t)12;        /* a(1)  */
    seq_t prev1 = (seq_t)36;        /* a(2)  */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = prev1 * (seq_t)3 + prev2 * (seq_t)3;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
