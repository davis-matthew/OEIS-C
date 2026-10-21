/*
 * A217529  a(n) = 2^(n-4)*(4*n^2 - 16*n + 23).
 * https://oeis.org/A217529
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4)*(4*n^2 - 16*n + 23)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 4u)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t term = 4u * nn * nn - 16u * nn + 23u;
    seq_t pw   = pow2((idx_t)(n - 4u));
    return pw * term;
}
