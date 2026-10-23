/*
 * A138513  a(n) = 8*a(n-1) - 5*a(n-2).
 * https://oeis.org/A138513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 5*a(n-2) Initial values: a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ implementation using std::vector for automatic growth and
       proper construction/destruction of arbitrary-precision objects. */
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        cache.reserve(2);
        cache.push_back(seq_t(1)); /* a(1) */
        cache.push_back(seq_t(3)); /* a(2) */
    }
    if (n <= cache.size())
        return cache[n-1];

    /* Grow to required size */
    cache.resize(n);
    for (size_t i = 2; i < static_cast<size_t>(n); ++i)
    {
        cache[i] = seq_t(8) * cache[i-1] - seq_t(5) * cache[i-2];
    }
    return cache[n-1];
#else
    /* C implementation using malloc/realloc.  seq_t is an unsigned integer
       type in the C build, so raw memory allocation suffices. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;

    if (cache == NULL)
    {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1; /* a(1) */
        cache[1] = 3; /* a(2) */
        cache_sz = 2;
    }

    if (n <= cache_sz)
        return cache[n-1];

    /* Grow capacity to at least n */
    if (n > cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_sz; i < (size_t)n; ++i)
    {
        cache[i] = 8 * cache[i-1] - 5 * cache[i-2];
    }
    cache_sz = (size_t)n;
    return cache[n-1];
#endif
}
