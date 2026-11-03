/*
 * A141722  a(n) = 8*4^n - 7.
 * https://oeis.org/A141722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*4^n - 7
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 906 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t val = ipow_u64_idx(4, n); /* compute 4^n */
    val *= (seq_t)8;            /* multiply by 8 */
    val -= (seq_t)7;            /* subtract 7 */
    return val;
}
