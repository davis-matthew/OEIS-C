/*
 * A013774  a(n) = 24^(3*n + 1).
 * https://oeis.org/A013774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 162 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Compute exponent e = 3*n + 1 */
    idx_t e = 3 * n + 1;
    return ipow_idx((seq_t)24, e);
}
