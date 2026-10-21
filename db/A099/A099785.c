/*
 * A099785  a(n) = Sum_{k=0..floor(n/4)} C(n-k,3*k) * 2^(n-3*k).
 * https://oeis.org/A099785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binom(n‑k,3k) · 2^{\,n‑3k} (recurrence:
 *            a(n)=6a(n‑1)−12a(n‑2)+8a(n‑3)+2a(n‑4),
 *            a(0)=1,a(1)=2,a(2)=4,a(3)=8)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;   /* number of terms already filled */
    static idx_t cache_cap  = 0;   /* current array capacity (in terms) */

    /* initialise cache with the first four values on the first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache_size = 4;                 /* 0..3 are valid */
    }

    /* enlarge the cache if the requested index is beyond its current capacity */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* make sure the four seed values are present if the cache has been contracted */
    if (cache_size < 4) {
        for (idx_t i = cache_size; i < 4; ++i) {
            switch (i) {
                case 0: cache[0] = (seq_t)1; break;
                case 1: cache[1] = (seq_t)2; break;
                case 2: cache[2] = (seq_t)4; break;
                case 3: cache[3] = (seq_t)8; break;
            }
        }
        cache_size = 4;
    }

    /* compute missing terms up to n (using the recurrence) */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 4) {                  /* seed values already set */
            continue;
        }
        /* a(i) = 6*a(i‑1) – 12*a(i‑2) + 8*a(i‑3) + 2*a(i‑4) */
        seq_t val = 6 * cache[i-1];
        val -= 12 * cache[i-2];
        val += 8 * cache[i-3];
        val += 2 * cache[i-4];
        cache[i] = val;
    }

    cache_size = n + 1;               /* the cache now contains 0..n */
    return cache[n];
}
