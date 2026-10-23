/*
 * A170529  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1, let b(n) = 2 for 1 ≤ n ≤ 45, b(n) = 1 for n
 *            = 0 or n = 46, otherwise b(n) = 0. Then a(n) = 38 ×
 *            (a(n-1)+a(n-2)+…+a(n-45)) – 741 × a(n-46) + b(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms in cache (next free index) */
    static size_t cache_cap = 0;      /* capacity of cache array */

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 64;                      /* start with some capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure - return 0 */
    }

    /* extend cache if required */
    if ((size_t)n + 1 > cache_len) {
        /* grow capacity */
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while ((size_t)n + 1 > new_cap) {
                new_cap *= 2;
            }
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL) return (seq_t)0;   /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t sum45 = 0;
            for (size_t j = 1; j <= 45; ++j) {
                if (i >= j) sum45 += cache[i - j];
            }
            seq_t val = sum45 * (seq_t)38;
            if (i >= 46) {
                val -= cache[i - 46] * (seq_t)741;
            }

            /* determine b(i) */
            seq_t b = 0;
            if (i == 0)          b = 1;
            else if (i <= 45)    b = 2;
            else if (i == 46)    b = 1;
            /* else b stays 0 */

            val += b;
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
