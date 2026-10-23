/*
 * A163348  a(n) = 6*a(n-1) - 7*a(n-2) for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A163348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) a(0) = 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for memoisation */
    static std::vector<seq_t> cache;

    /* Grow the cache until it contains (n + 1) entries. */
    while (cache.size() <= n)
    {
        if (cache.size() == 0)
            cache.push_back((seq_t)1);
        else if (cache.size() == 1)
            cache.push_back((seq_t)7);
        else
        {
            seq_t val = (seq_t)6 * cache[cache.size() - 1]
                        - (seq_t)7 * cache[cache.size() - 2];
            cache.push_back(val);
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Ensure the cache has enough capacity for (n + 1) entries */
    if (cache_cap <= n)
    {
        idx_t new_cap = cache_cap ? cache_cap * 2 : (n + 1);
        if (new_cap < (n + 1))
            new_cap = n + 1;           /* avoid overflow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                  /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n, storing them in the cache */
    while (cache_len <= n)
    {
        if (cache_len == 0)
            cache[0] = (seq_t)1;
        else if (cache_len == 1)
            cache[1] = (seq_t)7;
        else
        {
            cache[cache_len] = (seq_t)6 * cache[cache_len - 1]
                               - (seq_t)7 * cache[cache_len - 2];
        }
        ++cache_len;
    }

    return cache[(size_t)n];
#endif
}
