/*
 * A238389  Expansion of (1+x)/(1-x^2-3*x^3).
 * https://oeis.org/A238389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0)=a(1)=a(2)=1; for n>2, a(n) = a(n-2) + 3*a(n-3)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 44, u64 n <= 87, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* dynamic cache of computed values   */
    static size_t cache_len = 0;   /* number of elements currently stored */
    static size_t cache_cap = 0;   /* allocated capacity                 */

    /* Return the trivial initial values directly. */
    if (n < 3)
        return (seq_t)1;

    size_t idx = (size_t)n;

    /* If already cached, return it. */
    if (idx < cache_len)
        return cache[idx];

    /* Expand the cache if necessary. */
    size_t needed = idx + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed – abort (state is undefined otherwise) */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three entries if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_len = 3;
    }

    /* Compute missing terms up to the required index. */
    for (size_t i = cache_len; i <= idx; ++i) {
        cache[i] = cache[i-2] + (seq_t)3 * cache[i-3];
    }
    cache_len = needed;

    return cache[idx];
}
