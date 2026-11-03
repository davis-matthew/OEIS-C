/*
 * A226201  a(n) = 8^n + n.
 * https://oeis.org/A226201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 265 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t res = ipow_idx((seq_t)8, n);  /* 8^n */
    res += (seq_t)n;                   /* + n */
    return res;
}
