/*
 * A214825  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) = 1, a(1) = a(2) = 3.
 * https://oeis.org/A214825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = 1, a(1) = a(2) = 3
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

#ifndef __cplusplus
/* C implementation – use a dynamically resizable array allocated with malloc. */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation uses std::vector, which handles dynamic resizing
       automatically and correctly constructs sequence values. */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(16);
        cache.push_back((seq_t)1);   /* a(0) */
        cache.push_back((seq_t)3);   /* a(1) */
        cache.push_back((seq_t)3);   /* a(2) */
    }

    if (n < cache.size())
        return cache[(size_t)n];

    for (size_t i = cache.size(); i <= (size_t)n; ++i)
        cache.push_back(cache[i-1] + cache[i-2] + cache[i-3]);

    return cache[(size_t)n];
#else
    /* C implementation – no STL vector. */
    if (cache == NULL) {
        cache_cap = 16;
        cache_len = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)3;   /* a(1) */
        cache[2] = (seq_t)3;   /* a(2) */
    }

    if (n < cache_len)
        return cache[(size_t)n];

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i >= cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 16;
            while (newcap <= (size_t)i) newcap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) {    /* allocation failure */
                free(cache); cache = NULL; return (seq_t)0;
            }
            cache = tmp;
            cache_cap = newcap;
        }
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    cache_len = n + 1;
    return cache[(size_t)n];
#endif
}
