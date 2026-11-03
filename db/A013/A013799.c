/*
 * A013799  a(n) = 13^(4*n + 3).
 * https://oeis.org/A013799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13^(4*n+3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 187 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    idx_t e = n * 4 + 3;
    return ipow_u64_idx(13, e);
}
