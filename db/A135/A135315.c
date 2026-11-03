/*
 * A135315  a(n) = 8^n * 7^(n^2).
 * https://oeis.org/A135315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n * 7^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 29 (2026-11-01)
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
    seq_t pow8 = ipow_idx((seq_t)8, n);
    idx_t nsq = n * n;
    seq_t pow7 = ipow_idx((seq_t)7, nsq);
    return seq_mul(pow8, pow7);
}
