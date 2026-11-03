/*
 * A013847  a(n) = 8^(5*n + 2).
 * https://oeis.org/A013847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(5*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    idx_t e = 5 * n + 2;
    return ipow_idx((seq_t)8, e);
}
