/*
 * A017480  a(n) = (11*n + 7)^8.
 * https://oeis.org/A017480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 7)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 22, u128 n <= 5957,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 9922 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * n + 7U;
    return ipow_idx(base, 8U);
}
