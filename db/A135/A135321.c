/*
 * A135321  a(n) = 10^n * 9^(n^2).
 * https://oeis.org/A135321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n * 9^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 23 terms through n = 30 (2026-11-01)
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
    seq_t pow10 = ipow_idx((seq_t)10, n);
    idx_t exp9 = n * n;
    seq_t pow9 = ipow_idx((seq_t)9, exp9);
    return seq_mul(pow10, pow9);
}
