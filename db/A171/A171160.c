/*
 * A171160  a(n) = a(n-1) + 2*a(n-2) with a(0)=3, a(1)=4.
 * https://oeis.org/A171160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2), a(0)=3, a(1)=4
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 3;
    if (n == 1) return 4;

    seq_t prev2 = 3;   /* a(n-2) */
    seq_t prev1 = 4;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
