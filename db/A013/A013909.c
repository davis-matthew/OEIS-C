/*
 * A013909  a(n) = 23^(5*n + 4).
 * https://oeis.org/A013909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23^(5*n + 4)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 44 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    const seq_t base = 23;
    idx_t exp = 5*n + 4;
    return ipow_idx(base, exp);
}
