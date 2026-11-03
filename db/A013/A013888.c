/*
 * A013888  a(n) = 18^(5*n + 3).
 * https://oeis.org/A013888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    /* Exponent is 5*n + 3.  Use unsigned literals to avoid overflow. */
    idx_t exp = 5ULL * n + 3ULL;
    return ipow_u64_idx(18ULL, exp);
}
