/*
 * A042368  Numerators of continued fraction convergents to sqrt(711).
 * https://oeis.org/A042368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 160*a(n-4) - a(n-8) Initial values: a(0)=26, a(1)=27,
 *            a(2)=53, a(3)=80, a(4)=4213, a(5)=4293, a(6)=8506, a(7)=12799
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Vector for dynamic storage (C++ only) */
    static std::vector<seq_t> cache;
    static bool init = false;
    if (!init) {
        cache.resize(8);
        cache[0] = (seq_t)26;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)53;
        cache[3] = (seq_t)80;
        cache[4] = (seq_t)4213;
        cache[5] = (seq_t)4293;
        cache[6] = (seq_t)8506;
        cache[7] = (seq_t)12799;
        init = true;
    }

    size_t newSize = (size_t)n + 1;
    size_t oldSize = cache.size();
    if (newSize > oldSize)
        cache.resize(newSize);          /* extend storage */

    for (size_t i = oldSize; i <= (size_t)n; ++i) {
        /* recurrence holds for i >= 8 because oldSize ≥ 8 after init */
        cache[i] = (seq_t)160 * cache[i - 4] - cache[i - 8];
    }
    return cache[(size_t)n];
#else
    /* C implementation with malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* next free index */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise base values once */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed */
        cache[0] = (seq_t)26;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)53;
        cache[3] = (seq_t)80;
        cache[4] = (seq_t)4213;
        cache[5] = (seq_t)4293;
        cache[6] = (seq_t)8506;
        cache[7] = (seq_t)12799;
        cache_len = 8;
    }

    /* grow the array if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 8) continue;               /* already initialised */
        cache[i] = (seq_t)160 * cache[i - 4] - cache[i - 8];
    }
    if (cache_len <= (size_t)n) cache_len = (size_t)n + 1;
    return cache[(size_t)n];
#endif
}
