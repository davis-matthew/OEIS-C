/*
 * A013859  a(n) = 11^(5*n + 2).
 * https://oeis.org/A013859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(5*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 86 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute the exponent as a 64-bit integer */
    idx_t exp = 5 * n + 2;
    return ipow_u64_idx(11, exp);
}
