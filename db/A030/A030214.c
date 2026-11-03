/*
 * A030214  Expansion of eta(q^7)*eta(q^17).
 * https://oeis.org/A030214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in x * ∏_{k≥1}(1-x^(7k))(1-x^(17k))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9220 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

seq_t a(idx_t n)
{
    /* Natural offset: a(0) = 0 */
    if (n == 0)
        return 0;

    /* We need coefficients up to exponent n‑1. */
    idx_t size = n;                 /* array length: indices 0 … n‑1 */
    seq_t *dp  = alloc_seq(size);   /* dp[0] … dp[n‑1] */

    /* Initial polynomial is 1 (coefficient of x^0). */
    dp[0] = 1;
    for (idx_t i = 1; i < size; ++i) /* zero‑initialize the rest explicitly */
        dp[i] = 0;

    /* Process each factor (1 - x^(7k)) once. */
    for (idx_t k = 1; 7*k < size; ++k) {
        idx_t part = 7 * k;
        for (idx_t j = size - 1; j >= part; --j)
            dp[j] -= dp[j - part];
    }

    /* Process each factor (1 - x^(17k)) once.  Multiples of 119 occur twice. */
    for (idx_t k = 1; 17*k < size; ++k) {
        idx_t part = 17 * k;
        for (idx_t j = size - 1; j >= part; --j)
            dp[j] -= dp[j - part];
    }

    seq_t result = dp[size - 1];  /* coefficient of x^(n‑1) in the product */
    mem_free(dp);                     /* deallocate the auxiliary array */
    return result;
}
