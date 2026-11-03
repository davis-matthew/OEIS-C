/*
 * A220984  The right Aurifeuillian factor of 7^(14n+7) + 1.
 * https://oeis.org/A220984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(6n+3) + 7^(5n+3) + 3*7^(4n+2) + 7^(3n+2) + 3*7^(2n+1)
 *            + 7^(n+1) + 1
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* The base 7 is constant for all terms */
    const seq_t ONE = 1;
    const seq_t THREE = 3;
    const seq_t SEVEN = 7;

    /* Compute each part of the formula */
    seq_t p6 = ipow_idx(SEVEN, 6 * n + 3);   /* 7^(6n+3) */
    seq_t p5 = ipow_idx(SEVEN, 5 * n + 3);   /* 7^(5n+3) */
    seq_t p4 = seq_mul(THREE, ipow_idx(SEVEN, 4 * n + 2)); /* 3*7^(4n+2) */
    seq_t p3 = ipow_idx(SEVEN, 3 * n + 2);   /* 7^(3n+2) */
    seq_t p2 = seq_mul(THREE, ipow_idx(SEVEN, 2 * n + 1)); /* 3*7^(2n+1) */
    seq_t p1 = ipow_idx(SEVEN, n + 1);       /* 7^(n+1) */

    /* Sum all parts and add the constant 1 */
    seq_t result = p6 + p5 + p4 + p3 + p2 + p1 + ONE;
    return result;
}
