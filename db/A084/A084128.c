/*
 * A084128  a(n) = 4*a(n-1) + 4*a(n-2), a(0)=1, a(1)=2.
 * https://oeis.org/A084128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
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
    /* C++ implementation using std::vector for automatic memory management. */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(4);
        cache.push_back((seq_t)1); /* a(0) */
        cache.push_back((seq_t)2); /* a(1) */
    }
    while ((idx_t)cache.size() <= n) {
        idx_t i = cache.size();
        cache.push_back(cache[i-1] * (seq_t)4 + cache[i-2] * (seq_t)4);
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc for a static cache. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;     /* a(0) */
        cache[1] = (seq_t)2;     /* a(1) */
        cache_len = 2;
    }

    if ((idx_t)n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= (idx_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    while (cache_len <= (idx_t)n) {
        cache[cache_len] = cache[cache_len-1] * (seq_t)4 + cache[cache_len-2] * (seq_t)4;
        ++cache_len;
    }

    return cache[n];
#endif
}
