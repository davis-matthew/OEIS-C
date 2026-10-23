/*
 * A166456  Row sums of triangle A166455.
 * https://oeis.org/A166456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) - 2^popcount(n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t total = pow2(n + 1);

    idx_t tmp = n;
    idx_t pop = 0;
    while (tmp) {
        pop += tmp & 1;
        tmp >>= 1;
    }

    seq_t sub = pow2(pop);
    return total - sub;
}
