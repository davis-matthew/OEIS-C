/*
 * A152551  a(n) = (2*n+1)^floor((n-1)/2).
 * https://oeis.org/A152551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)^floor((n-1)/2)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 544 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    return ipow_u64_idx(2 * n + 1, (n - 1) / 2);
}
