/*
 * A084175  Jacobsthal oblong numbers.
 * https://oeis.org/A084175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1)+6*a(n-2)-8*a(n-3), a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses lazy memoisation with a dynamic array. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len   = 0;   /* number of entries currently stored       */
    static idx_t cache_cap   = 0;   /* allocated capacity of the cache array    */

    /* Helper: grow the cache to accommodate at least 'needed' elements. */
    if (cache_len > 0) {
        /* nothing to do */
    } else if (cache_len == 0) {
        /* initialise the first three values */
        if (!cache) {
            cache_cap = 4;                     /* a small starting capacity */
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cache_len = 3;
    }

    /* If the requested term is already known, just return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to fit the required index (n). */
    while (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        if (new_cap <= cache_cap) /* check for overflow */
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms, using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 3 * cache[i - 1] + 6 * cache[i - 2] - 8 * cache[i - 3];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
