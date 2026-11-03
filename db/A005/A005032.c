/*
 * A005032  a(n) = 7*3^n.
 * https://oeis.org/A005032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*3^n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 932 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    return (seq_t)7 * ipow_idx((seq_t)3, n);
}
