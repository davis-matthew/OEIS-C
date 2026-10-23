/*
 * A121256  a(n) = a(n-1)*a(n-3) - 1, starting with a(0)=a(1)=a(2)=2.
 * https://oeis.org/A121256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*a(n-3) - 1 Initial values: a(0) = 2, a(1) = 2,
 *            a(2) = 2 Computed iteratively with memoization, allows calls in
 *            any order
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache  = NULL;   /* array of cached values */
static size_t cache_cap = 0;   /* allocated capacity in elements      */
static size_t cache_computed = 0; /* number of elements actually computed (>=1) */

seq_t a(idx_t n)
{
    /* lazy initialization of the cache */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* in case of allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache_computed = 3; /* indices 0,1,2 are valid */
    }

    size_t idx = (size_t)n;   /* array index is size_t (fits idx_t). */

    /* compute up to requested index if necessary */
    if (idx >= cache_computed) {
        /* resize array if needed */
        if (idx >= cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= idx) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t one = (seq_t)1;
        /* extend sequence from first missing term up to idx */
        for (size_t i = cache_computed; i <= idx; ++i) {
            cache[i] = cache[i-1] * cache[i-3] - one;
        }
        cache_computed = idx + 1;
    }

    return cache[idx];
}
