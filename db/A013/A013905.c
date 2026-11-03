/*
 * A013905  a(n) = 22^(5n+4).
 * https://oeis.org/A013905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22^(5*n+4)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    idx_t exp = 5*n + 4;
    return ipow_u64_idx(22, exp);
}
