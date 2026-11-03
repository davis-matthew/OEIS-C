/*
 * A155609  a(n) = 4^n - 3^n + 1.
 * https://oeis.org/A155609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3^n + 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t p4 = ipow_u64_idx((uint64_t)4, n);
    seq_t p3 = ipow_u64_idx((uint64_t)3, n);
    return p4 - p3 + (seq_t)1;
}
