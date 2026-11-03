/*
 * A013861  a(n) = 11^(5*n+4).
 * https://oeis.org/A013861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(5*n+4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 96 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    idx_t exp = 5 * n + 4;
    seq_t base = 11;
    return ipow_idx(base, exp);
}
