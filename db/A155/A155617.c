/*
 * A155617  a(n) = 6^n + 4^n - 1.
 * https://oeis.org/A155617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + 4^n - 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 917 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow6 = ipow_idx((seq_t)6, n);
    seq_t pow4 = ipow_idx((seq_t)4, n);
    seq_t ans  = pow6 + pow4 - (seq_t)1;
    return ans;
}
