/*
 * A048904  Indices of heptagonal numbers (A000566) which are also octagonal.
 * https://oeis.org/A048904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 482*a(n-1) - a(n-2) - 144, a(1)=1, a(2)=345
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)345;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)345;
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)482 * prev1 - prev2 - (seq_t)144;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
