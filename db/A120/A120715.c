/*
 * A120715  Sequence produced by 14 X 14 Markov chain based on 14-vertex graph
 *          formed from direct product of two copies of the graph used in
 *          A120714.
 * https://oeis.org/A120715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies a linear recurrence of order 10:
 *   a(0) = 0
 *   For n ≥ 1 (and for n ≥ 10):
 *     a(n) = 2*a(n-1)+30*a(n-2)-6*a(n-3)-263*a(n-4)-250*a(n-5)
 *             +419*a(n-6)+666*a(n-7)+228*a(n-8)-28*a(n-9)-17*a(n-10)
 *   For 1 ≤ n ≤ 9 the terms are obtained from the generating function
 *   N(x)/D(x) where
 *     D(x) = 1 - 2x - 30x² + 6x³ + 263x⁴ + 250x⁵ - 419x⁶ - 666x⁷
 *            - 228x⁸ + 28x⁹ + 17x¹⁰
 *     N(x) = x·(27 + 784x + 1539x² - 3286x³ - 6475x⁴ - 1442x⁵
 *            - 3783x⁶ - 4444x⁷ - 986x⁸)
 *
 * The sequence starts: 0, 27, 838, 4025, 29742, …
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Coefficients of the denominator polynomial D(x) (excluding d0). */
    static const int d[11] = {
        0,   /* unused, d0 = 1 */
        -2,
        -30,
        6,
        263,
        250,
        -419,
        -666,
        -228,
        28,
        17
    };

    /* Coefficients of the numerator polynomial N(x) (shifted by one). */
    static const int c[11] = {
        0,    /* a(0) term is 0 */
        27,   /* x¹ */
        784,  /* x² */
        1539, /* x³ */
        -3286,/* x⁴ */
        -6475,/* x⁵ */
        -1442,/* x⁶ */
        -3783,/* x⁷ */
        -4444,/* x⁸ */
        -986  /* x⁹ */
    };

    /* Static cache for previously computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Initial allocation. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Ensure the cache can hold index n. */
    if (cache_len <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap = new_cap ? new_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Compute missing terms up to n. */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t sum_pos = 0, sum_neg = 0;
            /* Sum contributions from previous 10 terms. */
            for (int k = 1; k <= 10; ++k) {
                if ((idx_t)k > i) continue;
                seq_t val = cache[i - k];
                int dk = d[k];
                if (dk > 0)
                    sum_pos = sum_pos + mul_small(val, (unsigned)dk);
                else if (dk < 0)
                    sum_neg = sum_neg + mul_small(val, (unsigned)(-dk));
                /* dk == 0: nothing to do */
            }

            /* Coefficient from the numerator (non‑zero only for i ≤ 9). */
            seq_t coeff_pos = 0, coeff_neg = 0;
            if (i <= 9) {
                if (c[i] > 0) coeff_pos = (seq_t)c[i];
                else if (c[i] < 0) coeff_neg = (seq_t)(-c[i]);
            }

            /* a(i) = coeff + sum_neg - sum_pos. */
            seq_t res = sum_neg + coeff_pos;
            res = res - sum_pos;
            res = res - coeff_neg;
            cache[i] = res;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
