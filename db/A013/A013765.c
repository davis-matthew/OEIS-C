/*
 * A013765  a(n) = 19^(3*n + 2).
 * https://oeis.org/A013765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19^(3*n+2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 88 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    idx_t exp = n * 3 + 2;
    return ipow_u64_idx(19, exp);
}
