/*
 * A145028  a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4).
 * https://oeis.org/A145028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4), offset = 1; initial
 *            values a(1)=1, a(2)=2, a(3)=3, a(4)=4
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    if (n == 0)        /* safety for incomplete requests */
        return 0;

#if defined(__cplusplus)
    /* C++ implementation using std::vector, safe for arbitrary precision types */
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        cache.resize(4);
        cache[0] = 1; /* a(1) */
        cache[1] = 2; /* a(2) */
        cache[2] = 3; /* a(3) */
        cache[3] = 4; /* a(4) */
    }

    while (cache.size() < n)
    {
        const size_t sz = cache.size();
        seq_t sum = cache[sz - 1] + cache[sz - 2] + cache[sz - 3] + cache[sz - 4];
        cache.push_back(sum);
    }
    return cache[n - 1];
#else
    /* C implementation using a growable array of seq_t */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_size     = 0;
    if (cache_size == 0)                 /* first time: initialise the first four terms */
    {
        cache_capacity = 4;
        cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        cache[0] = 1; /* a(1) */
        cache[1] = 2; /* a(2) */
        cache[2] = 3; /* a(3) */
        cache[3] = 4; /* a(4) */
        cache_size  = 4;
    }

    while (cache_size < (size_t)n)
    {
        if (cache_size >= cache_capacity)     /* grow the buffer */
        {
            cache_capacity *= 2;
            cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        }
        /* compute a(cache_size+1) from the last four terms               *
         * cache indices are 0‑based, a(k) -> cache[k-1]                    */
        seq_t sum = cache[cache_size - 1] + cache[cache_size - 2]
                  + cache[cache_size - 3] + cache[cache_size - 4];
        cache[cache_size++] = sum;
    }

    return cache[n - 1];
#endif
}
