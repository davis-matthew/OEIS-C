/*
 * A034908  One half of octo-factorial numbers.
 * https://oeis.org/A034908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{j=1..n} (4*j-3) * 2^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset of the sequence is 1   */
    seq_t prod = 1;
    for (idx_t j = 1; j <= n; ++j) {
        prod *= (seq_t)(4 * j - 3);
    }
    return prod * pow2(n - 1);
}
