/*
 * A155653  a(n) = 10^n - 6^n + 1.
 * https://oeis.org/A155653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 6^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 876 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    seq_t p10 = ipow_u64_idx(10, n);
    seq_t p6  = ipow_u64_idx(6,  n);
    seq_t one = 1;
    return p10 - p6 + one;
}
