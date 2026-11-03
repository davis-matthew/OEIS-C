/*
 * A017470  a(n) = (11*n + 6)^10.
 * https://oeis.org/A017470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 6)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 647,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 9998 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* base = 11*n + 6 */
    seq_t base = (seq_t)11 * (seq_t)n + (seq_t)6;
    /* exponent 10 */
    return ipow_idx(base, 10);
}
