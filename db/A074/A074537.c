/*
 * A074537  a(n) = 2^n + 5^n + 6^n.
 * https://oeis.org/A074537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 5^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 200 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t p2 = ipow_idx(2, n);
    seq_t p5 = ipow_idx(5, n);
    seq_t p6 = ipow_idx(6, n);
    return p2 + p5 + p6;
}
