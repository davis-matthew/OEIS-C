/*
 * A155132  a(n) = 8*a(n-1) + 8*a(n-2), n > 2, a(0)=1, a(1)=7, a(2)=63.
 * https://oeis.org/A155132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 8*a(n-2) for n > 2; * a(0) = 1, a(1) = 7, a(2)
 *            = 63
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
    /* Return the initial terms directly. */
    if (n < 3) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)7;
            case 2: return (seq_t)63;
            default: return (seq_t)0;          /* unreachable */
        }
    }

#ifdef __cplusplus
    /* C++ implementation uses std::vector to handle arbitrary-precision types. */
    static std::vector<seq_t> cache;
    if (cache.size() == 0) {                     /* initialisation */
        cache.resize(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)63;
    }

    size_t idx = (size_t)n;
    if (idx < cache.size())
        return cache[idx];

    /* Compute terms until the requested index is reached. */
    for (size_t i = cache.size(); i <= idx; ++i) {
        seq_t val = (seq_t)8 * cache[i - 1] + (seq_t)8 * cache[i - 2];
        cache.push_back(val);
    }
    return cache[idx];

#else
    /* C implementation uses malloc/realloc.  Works for POD seq_t. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache_len == 0) {                         /* initialisation */
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)63;
        cache_len = 3;
    }

    size_t idx = (size_t)n;
    if (idx < cache_len)
        return cache[idx];

    /* Grow cache as needed and compute new terms. */
    while (cache_len <= idx) {
        if (cache_len == cache_cap) {             /* double capacity */
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len] = (seq_t)((seq_t)8 * cache[cache_len - 1]
                                 + (seq_t)8 * cache[cache_len - 2]);
        ++cache_len;
    }
    return cache[idx];
#endif
}
