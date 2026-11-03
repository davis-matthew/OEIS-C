/*
 * A164786  a(n) = 8^n-7.
 * https://oeis.org/A164786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n-7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 862 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) = 1) */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* compute 8^n */
    seq_t power = ipow_u64_idx(8, n);
    /* subtract 7 */
    return power - (seq_t)7;
}
