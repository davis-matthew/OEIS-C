/*
 * A170170  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence for A170170 Let b(n)=1 if n==0; 2 if 1≤n≤38; 1 if
 *            n==39; otherwise 0. a(0)=1. For n≥1: a(n) = 15⋅∑_{k=1}^{38}
 *            a(n‑k) – 120⋅a(n‑39) + b(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static inline seq_t b_value(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n <= 38ULL) return (seq_t)2;
    if (n == 39ULL) return (seq_t)1;
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached terms */
    static size_t cache_size = 0;    /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    size_t n_us = (size_t)n;

    /* Ensure enough space for term n */
    if (n_us >= cache_size) {
        size_t needed = n_us + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < needed) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n */
        for (size_t i = cache_size; i <= n_us; ++i) {
            seq_t val = 0;
            /* Sum the last up to 38 terms */
            size_t max_j = (i < 38) ? i : 38;
            for (size_t j = 1; j <= max_j; ++j) {
                val += cache[i - j];
            }
            /* Multiply by 15 */
            val *= (seq_t)15;
            /* Subtract 120*a(i-39) if i>=39 */
            if (i >= 39) {
                val -= (seq_t)120 * cache[i - 39];
            }
            /* Add b(i) */
            val += b_value((idx_t)i);
            cache[i] = val;
        }
        cache_size = n_us + 1;
    }

    return cache[n_us];
}
