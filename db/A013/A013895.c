/*
 * A013895  a(n) = 20^(5*n + 2).
 * https://oeis.org/A013895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20^(5*n + 2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 48 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 5*n + 2 */
    idx_t exp = 5 * n + 2;
    /* Raise 20 to that exponent */
    return ipow_u64_idx(20, exp);
}
