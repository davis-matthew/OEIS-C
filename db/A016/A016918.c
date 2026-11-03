/*
 * A016918  a(n) = (6*n)^10.
 * https://oeis.org/A016918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 14, u128 n <= 1188,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 1798 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * (seq_t)n;
    return ipow_idx(base, 10);
}
