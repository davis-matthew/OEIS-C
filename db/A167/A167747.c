/*
 * A167747  a(n) = phi(6^n).
 * https://oeis.org/A167747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(6^n) For n = 0, a(0) = 1 For n > 0, a(n) = 2^n *
 *            3^(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 928 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();
    seq_t two_pow   = ipow_i32((seq_t)2, (int)n);
    seq_t three_pow = ipow_i32((seq_t)3, (int)(n - 1));
    return seq_mul(two_pow, three_pow);
}
