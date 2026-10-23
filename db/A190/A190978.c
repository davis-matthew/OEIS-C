/*
 * A190978  a(n) = 8*a(n-1) - 6*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 6*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0; /* a(0) */
    seq_t prev1 = 1; /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = ((seq_t)8) * prev1 - ((seq_t)6) * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
