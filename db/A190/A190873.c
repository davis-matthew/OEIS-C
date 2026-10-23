/*
 * A190873  a(n) = 12*a(n-1) - 12*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A190873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 12*a(n-1) - 12*a(n-2); a(0)=0, a(1)=1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 12 * prev1 - 12 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
