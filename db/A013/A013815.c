/*
 * A013815  a(n) = 21^(4*n + 3).
 * https://oeis.org/A013815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21^(4*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 91 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 4*n + 3 using idx_t arithmetic. */
    idx_t exp = n * 4 + 3;
    /* Compute 21^exp in the seq_t type. */
    return ipow_idx((seq_t)21, exp);
}
