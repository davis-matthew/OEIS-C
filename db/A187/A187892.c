/*
 * A187892  a(0)=0, a(1)=6, a(n)=a(n-1)+a(n-2)-1.
 * https://oeis.org/A187892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=6, a(n)=a(n-1)+a(n-2)-1 (n≥2)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached values */
    static size_t cache_cap = 0;         /* number of valid entries */

    /* initialise cache with the two base cases */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)6;
    }

    size_t idx = (size_t)n;

    /* we already have the requested value */
    if (idx < cache_cap)
        return cache[idx];

    /* need to extend the cache to size idx+1 */
    size_t new_cap   = idx + 1;
    size_t old_cap   = cache_cap;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                        /* allocation failure – return safe value */
        return (seq_t)0;
    cache = tmp;
    cache_cap = new_cap;

    /* compute missing terms */
    for (size_t i = old_cap; i <= idx; ++i) {
        cache[i] = cache[i-1] + cache[i-2] - (seq_t)1;
    }

    return cache[idx];
}
