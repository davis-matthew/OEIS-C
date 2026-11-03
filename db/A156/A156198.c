/*
 * A156198  a(n) = 2*a(n-1)+3 with n>1, a(1)=8.
 * https://oeis.org/A156198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*2^(n-1)-3 (a(1)=8)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 997 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* compute 2^(n-1) */
    seq_t pow2 = ipow_idx((seq_t)2, n - 1);
    /* multiply by 11 and subtract 3 */
    seq_t res = pow2 * (seq_t)11 - (seq_t)3;
    return res;
}
