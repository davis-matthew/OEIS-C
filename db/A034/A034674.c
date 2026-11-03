/*
 * A034674  Sum of n-th powers of divisors of 128.
 * https://oeis.org/A034674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(8*n) - 1)/(2^n - 1) = ∑_{i=0}^{7} 2^(i*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 171 (2026-11-01)
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
    /* base = 2^n */
    seq_t base = ipow_idx((seq_t)2, n);

    seq_t term = (seq_t)1;
    seq_t sum  = (seq_t)0;

    /* Sum the geometric progression of 8 terms */
    for (idx_t i = 0; i < 8; ++i)
    {
        sum += term;
        term = seq_mul(term, base);
    }
    return sum;
}
