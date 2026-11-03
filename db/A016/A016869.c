/*
 * A016869  a(n) = (5n+1)^9.
 * https://oeis.org/A016869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n+1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)5 * n + 1);
    return ipow_idx(base, 9);
}
