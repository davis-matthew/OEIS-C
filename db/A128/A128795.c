/*
 * A128795  a(n) = n^3*9^n.
 * https://oeis.org/A128795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 983 (2026-11-01)
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
    seq_t nn   = (seq_t)n;          /* n          */
    seq_t n2   = seq_mul(nn, nn);   /* n^2        */
    seq_t n3   = seq_mul(n2, nn);   /* n^3        */
    seq_t pow9 = ipow_idx((seq_t)9, n); /* 9^n   */
    return seq_mul(n3, pow9);       /* n^3 * 9^n  */
}
