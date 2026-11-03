/*
 * A013787  a(n) = 7^(4*n + 3).
 * https://oeis.org/A013787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(4*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 199 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    idx_t exp = 4*n + 3;      /* exponent as idx_t */
    return ipow_u64_idx(7, exp); /* compute 7^exp exactly */
}
