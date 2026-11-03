/*
 * A122971  30th powers: a(n) = n^30.
 * https://oeis.org/A122971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = n^30
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 9909 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    return ipow_idx(base, 30);
}
