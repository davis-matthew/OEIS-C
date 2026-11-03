/*
 * A017040  a(n) = (7*n + 4)^12.
 * https://oeis.org/A017040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 5, u128 n <= 231,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 9425 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Compute base = 7*n + 4 */
    seq_t base = (seq_t)n * 7 + 4;

    /* Compute the 12th power of the base */
    return ipow_idx(base, 12);
}
