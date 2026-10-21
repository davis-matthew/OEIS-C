/*
 * A094033  Number of connected 2-element antichains on a labeled n-set.
 * https://oeis.org/A094033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n - 3*2^n + 3) / 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(0)=0, a(1)=0, a(2)=0, a(3)=3;
   for n ≥ 4:  a(n) = 6*a(n‑1) – 11*a(n‑2) + 6*a(n‑3) */
#include <stdlib.h>

/* the function must be exported with external linkage */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache   */
    static size_t cache_cap = 0;   /* allocated capacity of cache array  */

    /* initialise the cache on first entry */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0;   /* unlikely, but safe */

        /* seed known initial values */
        cache[0] = (seq_t)0; /* a(0) */
        cache[1] = (seq_t)0; /* a(1) */
        cache[2] = (seq_t)0; /* a(2) */
        cache[3] = (seq_t)3; /* a(3) */
        cache_len = 4;
    }
    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if ((size_t)n + 1 > new_cap) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0; /* unlikely, but safe */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* compute missing values up to n */
    if ((size_t)n >= cache_len) {
        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            if (k < 4) continue; /* values 0..3 already set */
            /* recurrence: a(k) = 6*a(k-1) - 11*a(k-2) + 6*a(k-3) */
            seq_t val = (seq_t)6 * cache[k-1]
                      - (seq_t)11 * cache[k-2]
                      + (seq_t)6 * cache[k-3];
            cache[k] = val;
        }
        cache_len = n + 1;
    }
    return cache[(size_t)n];
}
