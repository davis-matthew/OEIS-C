/*
 * A042720  Numerators of continued fraction convergents to sqrt(890).
 * https://oeis.org/A042720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 358*a(n-4) - a(n-8) with initial values a(0)=29, a(1)=30,
 *            a(2)=149, a(3)=179, a(4)=10531, a(5)=10710, a(6)=53371,
 *            a(7)=64081
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[8] = {
    (seq_t)29,  (seq_t)30,  (seq_t)149, (seq_t)179,
    (seq_t)10531, (seq_t)10710, (seq_t)53371, (seq_t)64081
};

seq_t a(idx_t n)
{
    /* Return the initial terms directly for n < 8 */
    if (n < 8)
        return init_terms[(idx_t)n];

    /* Static cache of already computed values */
    static seq_t *cache = NULL;   /* pointer to cached terms */
    static idx_t cache_size = 0;  /* cache capacity in terms of indices (i.e., size = max_index+1) */

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        /* Allocate room for the 8 initial terms plus one for language convenience */
        cache_size = 8;                 /* we store indices 0..7 */
        cache = (seq_t *)malloc((size_t)(cache_size) * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;            /* allocation failure – caller will interpret 0 as error */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init_terms[i];
    }

    /* Expand cache if the requested index is beyond the current capacity */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;            /* allocation failure */
        cache = tmp;

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = 358*a(i-4) - a(i-8)  ;  i >= 8 ensures indices are valid */
            cache[i] = cache[i - 4] * (seq_t)358 - cache[i - 8];
        }

        cache_size = new_size;
    }

    return cache[n];
}
