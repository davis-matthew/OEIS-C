/*
 * A013881  a(n) = 16^(5*n+4).
 * https://oeis.org/A013881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(5*n+4)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 88 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = 16^(5*n+4) for all n >= 0 using fast exponentiation. */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 5*n + 4 */
    idx_t exp = n * 5 + 4;
    /* Return 16 raised to that exponent */
    return ipow_u64_idx(16, exp);
}
