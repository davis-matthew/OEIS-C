/*
 * A220783  Agreement numbers: number of n X 2 arrays of the count of
 *          horizontal and vertical neighbors equal to the corresponding
 *          element in a random 0..3 n X 2 array.
 * https://oeis.org/A220783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2) - 4*a(n-3); Initial values: a(1) = 2,
 *            a(2) = 11, a(3) = 66
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* 1‑based cache of computed terms */
static size_t cache_cap = 0;   /* current capacity (>= last stored index + 1) */
static idx_t  last = 0;        /* highest n that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 0, but harmless */

    /* allocate and initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 4;                     /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */

        cache[1] = (seq_t)2;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)66;
        last    = 3;
    }

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap = new_cap * 2;
        }
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        /* copy existing values */
        for (idx_t i = 1; i <= last; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)5
                     + cache[i-2] * (seq_t)6
                     - cache[i-3] * (seq_t)4;
        }
        last = n;
    }

    return cache[n];
}
