/*
 * A099371  Expansion of g.f.: x/(1 - 9*x - x^2).
 * https://oeis.org/A099371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * a(n-1) + a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build – use std::vector to construct elements automatically */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(128);
        cache.push_back(0);      /* a(0) */
        cache.push_back(1);      /* a(1) */
    }

    /* grow the cache if required */
    if ((size_t)n + 1 > cache.size()) {
        cache.resize(n + 1);     /* new elements default‑constructed to 0 */
        for (size_t i = 2; i <= (size_t)n; ++i)
            cache[i] = 9 * cache[i-1] + cache[i-2];
    }
    return cache[n];
#else
    /* C build – manual dynamic array (values are plain integers) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;      /* number of valid entries */

    /* initialise on the first call */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* grow the allocated buffer if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[n % cache_cap];
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = 9 * cache[i-1] + cache[i-2];

    if (n + 1 > cache_len) cache_len = n + 1;

    return cache[n];
#endif
}
