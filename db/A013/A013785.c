/*
 * A013785  a(n) = 6^(4n+3).
 * https://oeis.org/A013785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(4n+3)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 200 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6;
    return ipow_idx(base, 4*n + 3);
}
