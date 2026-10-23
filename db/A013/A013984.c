/*
 * A013984  Expansion of 1/(1-x^2-x^3-x^4-x^5-x^6-x^7).
 * https://oeis.org/A013984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(n)=a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)
 *            (n≥2) (Number of compositions of n into parts 2..7)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 98, u128 n <= 193,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;           /* pointer to the cached values */
    static idx_t  cached_to = 0;          /* next index not yet computed */
    static size_t cache_cap = 0;          /* allocated capacity in elements */

    /* allocate or grow the cache, if needed */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while ((idx_t)(new_cap) <= n) new_cap *= 2;
        seq_t *tmp = NULL;
#ifdef __cplusplus
        tmp = (seq_t*)realloc(cache, sizeof(seq_t)*new_cap);
#else
        tmp = realloc(cache, sizeof(seq_t)*new_cap);
#endif
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill in missing values up to and including n */
    for (idx_t i = cached_to; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 1;               /* a(0) */
        } else if (i == 1) {
            cache[i] = 0;               /* a(1) */
        } else {
            seq_t sum = 0;
            if (i >= 2) sum += cache[i-2];
            if (i >= 3) sum += cache[i-3];
            if (i >= 4) sum += cache[i-4];
            if (i >= 5) sum += cache[i-5];
            if (i >= 6) sum += cache[i-6];
            if (i >= 7) sum += cache[i-7];
            cache[i] = sum;             /* a(i) */
        }
    }

    cached_to = n + 1;
    return cache[n];
}
