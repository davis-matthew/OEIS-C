/*
 * A098880  a(n) = 11^n * n^11.
 * https://oeis.org/A098880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n * n^11
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute 11^n and n^11 separately, then multiply. */
    seq_t pow11   = ipow_u64_idx(11, n);          /* 11^n   */
    seq_t pown11  = ipow_u64_idx((uint64_t)n, 11); /* n^11   */
    return pow11 * pown11;
}
