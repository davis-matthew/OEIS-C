/*
 * A051481  a(n+1) = a(n)^2 + n*(n+3)*a(n).
 * https://oeis.org/A051481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = a(n)^2 + (n+1)*(n+4)*a(n) (a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
# include <vector>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ version: use a std::vector for natural copy‑construction */
    static std::vector<seq_t> cache;
    if (cache.size() <= (size_t)n)
    {
        size_t old = cache.size();
        cache.resize((size_t)n + 1);
        for (size_t i = old; i <= (size_t)n; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)1;
            else
            {
                idx_t idx = (idx_t)i - 1;
                seq_t prev = cache[idx];
                seq_t tmp  = (seq_t)(idx + 1) * (seq_t)(idx + 4);
                cache[i] = prev * (prev + tmp);
            }
        }
    }
    return cache[(size_t)n];
#else
    /* C version: dynamic array with realloc */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* number of terms computed */
    static size_t capacity   = 0;   /* allocated capacity */

    if ((size_t)n + 1 > capacity)
    {
        size_t new_cap = capacity == 0 ? 8 : capacity * 2;
        while (new_cap < (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    while (cache_size <= n)
    {
        if (cache_size == 0)
        {
            cache[0] = (seq_t)1;
            ++cache_size;
            continue;
        }
        idx_t i = cache_size - 1;     /* current index (a_i) */
        seq_t prev = cache[i];
        /* (i+1)*(i+4) is n*(n+3) in the OEIS definition   */
        seq_t tmp  = (seq_t)(i + 1) * (seq_t)(i + 4);
        cache[cache_size] = prev * (prev + tmp);
        ++cache_size;
    }

    return cache[n];
#endif
}
