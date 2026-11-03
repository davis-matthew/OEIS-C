/*
 * A136524  a(n) = 2^n*(2^n + n)^(n-1).
 * https://oeis.org/A136524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (2^n + n)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 57 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();

    seq_t twoPow = pow2(n);                 /* 2^n  */
    seq_t base   = twoPow + (seq_t)n;       /* 2^n + n */
    seq_t powRes = ipow_idx(base, n - 1);    /* (2^n + n)^(n-1) */
    return seq_mul(twoPow, powRes);         /* 2^n * (2^n + n)^(n-1) */
}
