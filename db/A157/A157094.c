/*
 * A157094  Denominator of Euler(n, 1/27).
 * https://oeis.org/A157094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^v₂(n+1) * 3^(3n) where v₂(x) is the exponent of 2 in
 *            the prime factorisation of x
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
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
    /* 3^(3n) */
    seq_t res = ipow_idx((seq_t)3, (idx_t)(3 * n));

    /* Compute v₂(n+1) */
    idx_t m = n + 1;
    idx_t v2 = 0;
    while ((m & 1) == 0) {
        v2++;
        m >>= 1;
    }

    /* Multiply by 2^v₂(n+1) */
    seq_t factor = ipow_idx((seq_t)2, v2);
    return seq_mul(res, factor);
}
