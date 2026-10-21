/*
 * A078012  a(n) = a(n-1) + a(n-3) for n >= 3, with a(0) = 1, a(1) = a(2) = 0.
 *          This recurrence can also be used to define a(n) for n < 0.
 * https://oeis.org/A078012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3), a(0)=1, a(1)=0, a(2)=0
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 62, u64 n <= 120,
 *            u128 n <= 236, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Cache the computed values.  When the function is called in C++ we
       use std::vector so that class‑type `seq_t` objects are constructed
       correctly.  In C we fall back to manual malloc/realloc. */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;    /* allocated array of seq_t */
    static idx_t cache_len = 0;    /* number of valid elements in cache */
#endif

    /* Ensure the cache is long enough */
#ifdef __cplusplus
    const size_t old_sz = cache.size();
    if (cache.size() <= (size_t)n) cache.resize(n + 1);
#else
    const size_t old_sz = (size_t)cache_len;
    if (cache_len <= n) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }
#endif

    /* Compute from the first not-yet-computed index up to n */
    size_t i;
#ifdef __cplusplus
    for (i = old_sz; i <= (size_t)n; ++i) {
        if (i == 0)            cache[i] = (seq_t)1;
        else if (i == 1 || i == 2) cache[i] = (seq_t)0;
        else                    cache[i] = cache[i - 1] + cache[i - 3];
    }
#else
    for (i = old_sz; i <= (size_t)n; ++i) {
        if (i == 0)            cache[i] = (seq_t)1;
        else if (i == 1 || i == 2) cache[i] = (seq_t)0;
        else                    cache[i] = cache[i - 1] + cache[i - 3];
    }
#endif

    return cache[(size_t)n];
}
