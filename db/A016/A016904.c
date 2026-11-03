/*
 * A016904  a(n) = (5*n + 4)^8.
 * https://oeis.org/A016904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 50, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 917 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 5U + 4;
    return ipow_idx(base, 8);
}
