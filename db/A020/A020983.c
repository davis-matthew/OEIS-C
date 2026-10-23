/*
 * A020983  Expansion of 1/((1-9*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A020983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) - 318*a(n-2) + 1080*a(n-3), n >= 3 a(0) = 1,
 *            a(1) = 31, a(2) = 643
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
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
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        cache.reserve(3);
        cache.push_back((seq_t)1);
        cache.push_back((seq_t)31);
        cache.push_back((seq_t)643);
    }
    if ((size_t)n < cache.size())
        return cache[(size_t)n];

    for (size_t i = cache.size(); i <= (size_t)n; ++i)
    {
        seq_t term1 = 31 * cache[i-1];
        seq_t term2 = 318 * cache[i-2];
        seq_t term3 = 1080 * cache[i-3];
        cache.push_back(term1 - term2 + term3);
    }
    return cache[(size_t)n];
#else
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;

    if (cache == NULL)
    {
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)643;
        cache_size = 3;
    }

    if (n >= cache_size)
    {
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed)
        {
            size_t new_cap = needed * 2;
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_size; i <= n; ++i)
        {
            seq_t term1 = 31 * cache[i-1];
            seq_t term2 = 318 * cache[i-2];
            seq_t term3 = 1080 * cache[i-3];
            cache[i] = term1 - term2 + term3;
        }
        cache_size = n + 1;
    }
    return cache[(size_t)n];
#endif
}
