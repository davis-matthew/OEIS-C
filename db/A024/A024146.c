/*
 * A024146  a(n) = 12^n - n^6.
 * https://oeis.org/A024146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^n - n^6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 295 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t p12 = ipow_idx((seq_t)12, n);
    seq_t p6  = ipow_idx((seq_t)n, (idx_t)6);
    return p12 - p6;
}
