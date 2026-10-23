/*
 * A042438  Numerators of continued fraction convergents to sqrt(747).
 * https://oeis.org/A042438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 164*a(n-2) - a(n-4) * Initial values: a(0)=27, a(1)=82,
 *            a(2)=4455, a(3)=13447
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t INIT[4] = {27, 82, 4455, 13447};

seq_t a(idx_t n)
{
    /* Handle the first four terms directly.  They never need caching. */
    if (n < 4)
        return INIT[(size_t)n];

    /* Static cache for memoisation. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity (number of elems) */
    static size_t cache_last = 0;   /* largest index that has been computed */

    /* First call – allocate and initialise the cache with the first four terms. */
    if (cache == NULL) {
        /* Allocate at least enough for the requested index and the four initial terms. */
        cache_cap = (size_t)n + 1;
        if (cache_cap < 4)
            cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return INIT[0];            /* OOM – fall back to a safe value. */
        cache[0] = INIT[0];
        cache[1] = INIT[1];
        cache[2] = INIT[2];
        cache[3] = INIT[3];
        cache_last = 3;
    }

    /* If the cache is too small, grow it. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;               /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp != NULL) {
            cache     = tmp;
            cache_cap = new_cap;
        }
        /* If realloc fails we continue with the old capacity, which will
           still correctly answer the current query if n < old capacity.  */
    }

    /* If the requested term was already computed, return it. */
    if ((size_t)n <= cache_last)
        return cache[n];

    /* Compute missing terms up to n.  We can rely on recurrence using indices
       strictly less than the current i. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* Compute: a(i) = 164*a(i-2) - a(i-4) */
        seq_t val = cache[i - 2];
        val *= (seq_t)164;        /* multiplication by the small integer 164 */
        val -= cache[i - 4];
        cache[i] = val;
    }
    cache_last = n;
    return cache[n];
}
