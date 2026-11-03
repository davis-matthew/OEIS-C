/*
 * A017051  a(n) = (7*n + 5)^11.
 * https://oeis.org/A017051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 454,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 9879 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t s = (seq_t)n;
    seq_t base = (seq_t)7 * s + 5;
    return ipow_idx(base, (idx_t)11);
}
