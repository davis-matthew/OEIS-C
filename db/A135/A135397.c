/*
 * A135397  a(n) = 2^n * 3^(n^2).
 * https://oeis.org/A135397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * 3^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    idx_t sq = n * n;                       /* n^2 */
    seq_t pow2 = ipow_idx((seq_t)2, n);       /* 2^n   */
    seq_t pow3 = ipow_idx((seq_t)3, sq);      /* 3^(n^2) */
    return seq_mul(pow2, pow3);              /* product */
}
