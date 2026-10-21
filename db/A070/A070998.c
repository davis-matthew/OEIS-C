/*
 * A070998  a(n) = 9*a(n-1) - a(n-2) for n > 0, a(0)=1, a(-1)=1.
 * https://oeis.org/A070998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 8, a(n) = 9*a(n-1) - a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
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
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(2);
        cache.push_back(1);   /* a(0) */
        cache.push_back(8);   /* a(1) */
    }

    if (n < static_cast<idx_t>(cache.size()))
        return cache[n];

    for (idx_t i = static_cast<idx_t>(cache.size()); i <= n; ++i) {
        cache.push_back(9 * cache[i - 1] - cache[i - 2]);
    }
    return cache[n];
#else
    /* C implementation using dynamic array */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;        /* a(0) */
        cache[1] = 8;        /* a(1) */
        cache_len = 2;
    }

    if (n < cache_len)
        return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i >= cache_cap) {
            idx_t newcap = cache_cap * 2;
            if (newcap < 4) newcap = 4;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cache_cap = newcap;
        }
        cache[i] = 9 * cache[i - 1] - cache[i - 2];
    }
    cache_len = n + 1;
    return cache[n];
#endif
}
