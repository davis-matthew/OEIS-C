/*
 * A166971  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 1, a(n) = 13·(a(n−1)+a(n−2)+…+a(n−12)) – 91·a(n−13) + b(n)
   for n ≥ 1 with the boundary values
        b(0) = 1,   b(1)..b(12) = 2,   b(13) = 1,   b(n) = 0 for n>13
   This reproduces the OEIS sequence A166971. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms, cached[0..cache_len-1] contains a(0)…a(cache_len-1). */
    static seq_t *cached = NULL;
    static size_t cache_len = 0;   /* Number of terms currently stored. */
    static size_t cache_cap = 0;   /* Current allocation size. */

    /* Ensure we have a non‑empty cache holding a(0) at least. */
    if (cache_len == 0) {
        size_t init_cap = 16;
        cached = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (!cached) return (seq_t)0;   /* allocation failure */
        cache_cap = init_cap;
    }

    /* Grow the cache to hold terms up to n, computing missing terms. */
    if ((idx_t)cache_len <= n) {
        /* Compute missing terms from cache_len to n inclusive. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            /* Sum the 12 preceding terms, if they exist. */
            for (idx_t j = 1; j <= 12 && j <= i; ++j) {
                sum += (seq_t)13 * cached[i - j];
            }

            /* Subtract 91·a(i−13) when available. */
            if (i >= 13) {
                sum -= (seq_t)91 * cached[i - 13];
            }

            /* Add the numerator contribution b(i). */
            if (i == 0) {
                sum += (seq_t)1;
            } else if (i <= 12) {
                sum += (seq_t)2;
            } else if (i == 13) {
                sum += (seq_t)1;
            }

            /* Ensure sufficient capacity for the new term. */
            if ((size_t)(i + 1) > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                if (new_cap < (size_t)(i + 1)) {
                    new_cap = (size_t)(i + 1);
                }
                seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failure */
                cached = tmp;
                cache_cap = new_cap;
            }

            cached[i] = sum;
        }
        cache_len = n + 1;
    }

    return cached[n];
}
