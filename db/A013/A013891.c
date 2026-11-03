/*
 * A013891  a(n) = 19^(5*n + 2).
 * https://oeis.org/A013891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19^(5*n+2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Return 19 raised to the exponent 5*n+2 */
    return ipow_u64_idx(19, 5 * n + 2);
}
