/*
 * A013911  a(n) = 24^(5*n + 2).
 * https://oeis.org/A013911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^(5*n + 2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    idx_t exp = (idx_t)(5*n + 2);
    return ipow_idx((seq_t)24, exp);
}
