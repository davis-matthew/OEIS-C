/*
 * A173155  a(n) = binomial(n + 5, 5) * 8^n.
 * https://oeis.org/A173155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + 5, 5)*8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 383 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    seq_t bin = binomial(n + 5, 5);
    seq_t pow = ipow_u64_idx(8, n);
    return seq_mul(bin, pow);
}
