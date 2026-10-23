/*
 * A138573  a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4); a(0)=0, a(1)=1,
 *          a(2)=2, a(3)=5.
 * https://oeis.org/A138573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4);
 * a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 5
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0; /* allocated capacity */
    static size_t cache_len = 0; /* number of values stored (highest index + 1) */

    /* fast path for the first four terms */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)5;
            default: break;
        }
    }

    /* ensure we have enough space for indices up to n */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_cap];
        if (!tmp) return (seq_t)0; /* allocation failed */
        /* copy existing data */
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete [] cache;
#else
        tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
#endif
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base values if this is the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache_len = 4;
    }

    /* compute any new terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* i >= 4 here */
        cache[i] = (2 * cache[i-1] + 2 * cache[i-2] + 2 * cache[i-3]) - cache[i-4];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
