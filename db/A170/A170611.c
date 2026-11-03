/*
 * A170611  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 26 * 25^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper functions from the utils library */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    if (n == 0U)
        return seq_one();                 /* a(0) = 1 */

    /* For n >= 1: a(n) = 26 * 25^(n-1) */
    return seq_mul(26,                        /* base 26 */
                   ipow_idx(25, n - 1));      /* 25^(n-1) */
}
