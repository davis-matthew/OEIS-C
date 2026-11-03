/*
 * A013819  a(n) = 23^(4*n + 3).
 * https://oeis.org/A013819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23^(4*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 81 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* compute exponent: 4*n + 3 */
    idx_t exp = n << 2;   /* 4*n */
    exp += 3;
    return ipow_u64_idx(23, exp);
}
