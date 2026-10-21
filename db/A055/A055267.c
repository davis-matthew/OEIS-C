/*
 * A055267  a(n) = 3*a(n-1) - a(n-2) with a(0)=1, a(1)=7.
 * https://oeis.org/A055267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n) {
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.push_back((seq_t)1); /* a(0) */
        cache.push_back((seq_t)7); /* a(1) */
    }
    while (cache.size() <= n) {
        size_t sz = cache.size();           /* current size */
        /* sz >= 2 at this point, so sz-1 and sz-2 are valid */
        seq_t val = (seq_t)3 * cache[sz-1] - cache[sz-2];
        cache.push_back(val);
    }
    return cache[(size_t)n];
#else
    static idx_t cache_len = 0;
    static seq_t *cache = NULL;

    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* enlarge the cache to hold indices up to n */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache,
                             (size_t)new_len * sizeof(seq_t));
    /* fill from old length to n */
    idx_t i, old_len = cache_len;
    for (i = old_len; i <= n; ++i) {
        if (i == 0) {
            cache[(size_t)i] = (seq_t)1; /* a(0) */
        } else if (i == 1) {
            cache[(size_t)i] = (seq_t)7; /* a(1) */
        } else {
            cache[(size_t)i] =
                (seq_t)3 * cache[(size_t)(i-1)] -
                cache[(size_t)(i-2)];
        }
    }
    cache_len = new_len;
    return cache[(size_t)n];
#endif
}
