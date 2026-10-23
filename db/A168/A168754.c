/*
 * A168754  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the recurrence * * a(n) = 27·∑_{k=1}^{17} a(n-k)
 *            – 378·a(n-18) + p_n (n ≥ 0), * * where the "forcing" terms p_n
 *            are the coefficients of the numerator * * p(t) = 1 + 2·t + 2·t²
 *            + … + 2·t¹⁷ + t¹⁸. * * Initial values: a(0) = 1. The recurrence
 *            automatically * generates all subsequent t...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;   /* values already computed (size = cache_len) */
    static size_t cache_len = 0;  /* number of elements currently stored */
    static size_t cache_cap = 0;  /* allocated capacity (in elements) */

    /* initialise cache with a(0) */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_len = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        if (new_len > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_len;
        }

        /* compute new terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t val = 0;

            /* 27 * sum_{k=1}^{17} a(i-k) */
            size_t maxk = (i < 17) ? i : 17;
            for (size_t k = 1; k <= maxk; ++k) {
                val += (seq_t)27 * cache[i - k];
            }

            /* -378 * a(i-18) if i≥18 */
            if (i >= 18) {
                val -= (seq_t)378 * cache[i - 18];
            }

            /* add forcing term p_i */
            if (i == 0) {
                val += (seq_t)1;
            } else if (i == 18) {
                val += (seq_t)1;
            } else if (i <= 17) {
                val += (seq_t)2;
            }

            cache[i] = val;
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
