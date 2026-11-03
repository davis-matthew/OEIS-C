/*
 * A172501  a(n) = binomial(n+8,8)*6^n.
 * https://oeis.org/A172501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) * 6^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 400 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t b = binomial(n + 8, 8);          /* binomial coefficient */
    seq_t p = ipow_idx((seq_t)6, n);     /* 6^n */
    return b * p;                       /* product */
}
