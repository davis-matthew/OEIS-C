/*
 * A166538  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*∑_{j=1}^{11} a(n-j) - 21*a(n-12) for n ≥ 13 Initial
 *            values (a(0) … a(12)) are given below
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache up to the largest n requested (limit 10000) */
    static seq_t cache[10001];
    static size_t cache_len = 0;  /* number of values computed */

    /* initial 13 terms – the only non‑homogeneous part occurs at n = 12 */
    if (cache_len == 0) {
        const seq_t init[13] = {
            (seq_t)1,          /* a(0) */
            (seq_t)8,          /* a(1) */
            (seq_t)56,         /* a(2) */
            (seq_t)392,        /* a(3) */
            (seq_t)2744,       /* a(4) */
            (seq_t)19208,      /* a(5) */
            (seq_t)134456,     /* a(6) */
            (seq_t)941192,     /* a(7) */
            (seq_t)6588344,    /* a(8) */
            (seq_t)46118408,   /* a(9) */
            (seq_t)322828856,  /* a(10) */
            (seq_t)2259801992,/* a(11) */
            (seq_t)15818613916/* a(12) */
        };
        for (size_t i = 0; i < 13; ++i) cache[i] = init[i];
        cache_len = 13;
    }

    /* if the requested index is already computed, return it */
    if ((size_t)n < cache_len) return cache[n];

    /* ensure we do not exceed the predefined maximum */
    if ((size_t)n >= 10001) {
        /* maximum n expected is 10000 – out of bounds otherwise */
        return 0; /* or handle error in your framework */
    }

    /* compute subsequent values up to n (inclusive) */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t j = 1; j <= 11; ++j) {
            sum += cache[i - j];
        }
        cache[i] = sum * (seq_t)6 - cache[i - 12] * (seq_t)21;
    }
    cache_len = n + 1;
    return cache[n];
}
