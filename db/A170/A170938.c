/*
 * A170938  a(n) = 4^n+2^n+2.
 * https://oeis.org/A170938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 2^n + 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow4 = ipow_idx((seq_t)4, n);
    seq_t pow2 = ipow_idx((seq_t)2, n);
    return pow4 + pow2 + (seq_t)2;
}
