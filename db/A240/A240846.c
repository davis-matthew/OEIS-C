/*
 * A240846  a(0)=0, a(1)=1, a(n) = a(n-1)*12 + 13.
 * https://oeis.org/A240846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=a(n-1)*12+13 for n≥2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;   /* size of allocated array            */
    static idx_t cache_max   = 0;   /* largest index already computed     */

    /* First call – allocate and initialise up to n */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed            */
        cache[0] = 0;
        if (n >= 1) cache[1] = 1;
        for (idx_t i = 2; i <= n; ++i)
            cache[i] = cache[i-1] * 12 + 13;
        cache_max = n;
        return cache[n];
    }

    /* Value already cached – return it */
    if (n <= cache_max)
        return cache[n];

    /* Grow the cache if it is too small */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failed            */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new entries using the recurrence, but honour the
       special base case a(1)=1 when transitioning from 0 */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = 1;
        else
            cache[i] = cache[i-1] * 12 + 13;
    }

    cache_max = n;
    return cache[n];
}
