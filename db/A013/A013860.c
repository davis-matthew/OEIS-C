/*
 * A013860  a(n) = 11^(5*n + 3).
 * https://oeis.org/A013860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 96 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    idx_t exp = 5 * n + 3;
    return ipow_u64_idx(11, exp);
}
