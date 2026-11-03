/*
 * A013776  a(n) = 2^(4*n+1).
 * https://oeis.org/A013776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4*n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 188 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute exponent 4*n+1 safely */
    idx_t exp = (n << 2) + 1;
    return pow2(exp);
}
