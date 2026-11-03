/*
 * A123072  Bishops on an 8n+1 X 8n+1 board (see Robinson paper for details).
 * https://oeis.org/A123072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1, n-1) * (2*n)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* compute (2*n)! */
    idx_t double_n = n + n; /* 2*n fits in idx_t (uint64_t) */
    seq_t fact = factorial(double_n);

    /* compute binomial(2*n-1, n-1) */
    idx_t two_minus_one = double_n - (idx_t)1;
    seq_t bin = binomial(two_minus_one, n - (idx_t)1);

    /* multiply them together */
    return seq_mul(fact, bin);
}
