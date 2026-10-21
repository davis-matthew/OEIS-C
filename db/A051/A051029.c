/*
 * A051029  Ramanujan's b-series: expansion of
 *          (2-26x-12x^2)/(1-82x-82x^2+x^3).
 * https://oeis.org/A051029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=138, a(2)=11468, * a(n) = 82*a(n-1) + 82*a(n-2) -
 *            a(n-3) for n ≥ 3 * generated from the rational G.f.
 *            (2-26x-12x²)/((1+x)(1-83x+x²))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
static void ensure_cache(idx_t n)
{
    if (cache.size() <= n)
    {
        if (cache.empty())
        {
            cache.reserve(n + 1);
            cache.push_back((seq_t)2);       // a(0)
            cache.push_back((seq_t)138);     // a(1)
            cache.push_back((seq_t)11468);   // a(2)
        }
        for (idx_t i = cache.size(); i <= n; ++i)
        {
            seq_t term = (seq_t)82 * cache[i - 1] + (seq_t)82 * cache[i - 2] - cache[i - 3];
            cache.push_back(term);
        }
    }
}
#else
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static void ensure_cache(idx_t n)
{
    if (!cache)
    {
        idx_t init_size = 3;
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        cache[0] = (seq_t)2;       // a(0)
        cache[1] = (seq_t)138;     // a(1)
        cache[2] = (seq_t)11468;   // a(2)
        cache_size = init_size;
    }
    if (cache_size <= n)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i)
        {
            seq_t term = (seq_t)82 * cache[i - 1] + (seq_t)82 * cache[i - 2] - cache[i - 3];
            cache[i] = term;
        }
        cache_size = new_size;
    }
}
#endif

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
