/*
 * A052970  Expansion of (1-2*x)/(1-2*x-2*x^2+2*x^3).
 * https://oeis.org/A052970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+3) = 2*a(n+2) + 2*a(n+1) – 2*a(n) with a(0) = 1, a(1) = 0,
 *            a(2) = 2. This recurrence is derived from the rational
 *            generating function (1‑2x)/(1‑2x‑2x²+2x³)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* current allocated size */
    static idx_t cache_len = 0;   /* number of valid entries */

    /* initialise the cache on first call */
    if (cache_cap == 0) {
        cache_cap = 64;                       /* initial capacity */
        cache = (seq_t*)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory: return 0 */
        cache_len = 0;
    }

    /* ensure the first three terms are present */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache_len = 3;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* enlarge the cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + 2*a(i-2) - 2*a(i-3) */
        seq_t term1 = cache[i - 1];
        seq_t term2 = cache[i - 2];
        seq_t term3 = cache[i - 3];
        cache[i] = (seq_t)(2 * term1 + 2 * term2 - 2 * term3);
    }
    cache_len = n + 1;

    return cache[n];
}
