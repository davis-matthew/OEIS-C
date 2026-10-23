/*
 * A168234  A138100(n) + A168142(n).
 * https://oeis.org/A168234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   * a(n) = (4*k^3 + 2*k + 3) / 3 * where k is the smallest integer
 *            ≥1 such that * n ≤ k*(k+1)*(2*k+1)/3 (i.e. n lies in block k). *
 *            Block k contains 2*k^2 terms all equal to that value
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   prefix_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prefix_block.h"

seq_t a(idx_t n)
{
    /* find block index k with prefix_block(k-1) < n ≤ prefix_block(k) */
    idx_t low = 1, high = 1;
    while (prefix_block(high) < n)
        high <<= 1;          /* high stays well below 1<<32 in practice */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (prefix_block(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;

    /* compute (4*k^3 + 2*k + 3)/3 in seq_t */
    seq_t kk  = (seq_t)k;
    seq_t kk2 = kk * kk;
    seq_t kk3 = kk2 * kk;
    seq_t val = (seq_t)((4 * kk3 + 2 * kk + 3) / 3);
    return val;
}
