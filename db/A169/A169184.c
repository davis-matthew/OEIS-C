/*
 * A169184  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = 25·∑_{k=1}^{min(26,n)} a(n‑k) + (n ≥
 *            27 ? 325·a(n‑27) : 0) + c(n) where c(n) = 2 for 1≤n≤26, 1 for
 *            n=27, and 0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoised computation of the sequence. */
seq_t a(idx_t n)
{
    /* A static cache is used to store already-computed terms. */
    const size_t MAX_CACHE = 20000;                  /* enough for n≈10 000 */

    static seq_t *cache  = NULL;                     /* pointer to cache     */
    static size_t cache_len = 0;                     /* number of stored terms */
    static size_t cache_cap = 0;                     /* allocated capacity    */

    /* Special-case n == 0 to avoid allocating memory unnecessarily. */
    if (n == 0)
        return (seq_t)1;

    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache_cap = MAX_CACHE;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)              /* allocation failed – undefined */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Extend the cache up to the required index n. */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        if (new_len > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < new_len) new_cap <<= 1;   /* double until enough */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)                      /* reallocation failed – undefined */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing terms. */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t sum = (seq_t)0;
            /* 25·∑_{k=1}^{min(26,i)} a(i‑k) */
            for (size_t k = 1; k <= 26 && k <= i; ++k)
                sum += (seq_t)25 * cache[i - k];
            /* 325·a(i‑27) when i≥27 */
            if (i >= 27)
                sum += (seq_t)325 * cache[i - 27];

            /* Add the non‑homogeneous part c(i). */
            seq_t c = 0;
            if (i == 0)
                c = 1;
            else if (i <= 26)
                c = 2;
            else if (i == 27)
                c = 1;

            cache[i] = sum + c;
        }
        cache_len = new_len;
    }

    return cache[n];
}
