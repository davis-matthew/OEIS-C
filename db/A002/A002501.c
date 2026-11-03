/*
 * A002501  a(n) = 7^n - 3*4^n + 2*3^n.
 * https://oeis.org/A002501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 3*4^n + 2*3^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 196 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t pow7 = ipow_idx((seq_t)7, n);
    seq_t pow4 = ipow_idx((seq_t)4, n);
    seq_t pow3 = ipow_idx((seq_t)3, n);
    return pow7 + (seq_t)2 * pow3 - (seq_t)3 * pow4;
}
