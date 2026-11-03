/*
 * A003946  Expansion of (1+x)/(1-3*x).
 * https://oeis.org/A003946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, walk
 * Formula:   a(n) = 4*3^(n-1) for n >= 1, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 179 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return 4 * ipow_idx(3, n - 1);
}
