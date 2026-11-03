/*
 * A013780  a(n) = 4^(4*n + 1).
 * https://oeis.org/A013780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 196 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    return ipow_u64_idx(4, 4*n + 1);
}
