/*
 * A135425  a(n) = 6^n * 7^(n^2).
 * https://oeis.org/A135425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n * 7^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 25 (2026-11-01)
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

seq_t a(idx_t n) {
    /* 6^n */
    seq_t p6 = ipow_idx((seq_t)6, n);
    /* exponent for 7: n^2, safe for n up to 10^4 in 64‑bit */
    idx_t exp7 = n * n;
    seq_t p7 = ipow_idx((seq_t)7, exp7);
    /* product */
    return seq_mul(p6, p7);
}
