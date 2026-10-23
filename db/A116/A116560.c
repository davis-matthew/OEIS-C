/*
 * A116560  Expansion of g.f. x*(1+x+x^4+x^5-x^6+x^8+x^9)/(1-2*x^6-x^12).
 * https://oeis.org/A116560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*a(n-6) + a(n-12), a(0..11) = 0,1,1,0,0,1,1,1,2,1,1,2
 * Range:     u8 n <= 43, u16 n <= 79, u32 n <= 156, u64 n <= 307,
 *            u128 n <= 607, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[12] = {
    (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)0, (seq_t)0,
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)1,
    (seq_t)1, (seq_t)2
};

static seq_t *cache = NULL;        /* array holding computed values      */
static size_t   cache_size = 0;    /* number of slots currently allocated */
static size_t   computed_up_to = (size_t)-1;  /* last index that has a valid value */

seq_t a(idx_t n)
{
    size_t i = (size_t)n;

    /* Initialise the cache if it does not yet exist.  */
    if (cache == NULL) {
        /* Allocate at least 12 slots.  */
        cache_size = (i < 12 ? 12 : i + 1);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – give up */

        /* Copy the first 12 known terms.  */
        for (size_t j = 0; j < 12; ++j)
            cache[j] = init[j];

        /* Zero remaining slots (they will be overwritten when needed). */
        for (size_t j = 12; j < cache_size; ++j)
            cache[j] = (seq_t)0;

        computed_up_to = 11;   /* we have a(0)..a(11) */
    }

    /* If the request is before we have filled the cache, just return. */
    if (i <= computed_up_to && i < cache_size)
        return cache[i];

    /* Grow the cache if necessary. */
    if (i >= cache_size) {
        size_t new_size = i + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure – give up */

        /* Copy existing values. */
        for (size_t j = 0; j < cache_size; ++j)
            new_cache[j] = cache[j];

        /* Zero the new region (will be overwritten as we compute). */
        for (size_t j = cache_size; j < new_size; ++j)
            new_cache[j] = (seq_t)0;

        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute any missing values up to i. */
    size_t start = (computed_up_to + 1 > 12 ? computed_up_to + 1 : 12);
    for (size_t k = start; k <= i; ++k) {
        cache[k] = cache[k - 6] + cache[k - 6] + cache[k - 12];
    }

    computed_up_to = i;
    return cache[i];
}
