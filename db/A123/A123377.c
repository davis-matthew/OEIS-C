/*
 * A123377  Values X satisfying the equation 5(X-Y)^4 - XY = 0, where X >= Y.
 * https://oeis.org/A123377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 0, a(1) = 10, a(2) = 2916, a(3) = 933470 * For n >= 4:
 *            a(n) = 340*a(n-1) - 5798*a(n-2) + 340*a(n-3) - a(n-4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* memcpy */

static seq_t *cache = NULL;     /* cached values */
static idx_t  cache_size = 0;   /* number of cached elements */

seq_t a(idx_t n)
{
    /* Return immediately if already computed */
    if (n < cache_size)
        return cache[n];

    /* Determine size for new cache */
    size_t new_size = (size_t)n + 1;

    /* Allocate new cache */
    seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return 0;  /* out of memory – return 0 as a safe failure */

    /* Copy any existing cached values to the new array */
    if (cache && cache_size > 0) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);          /* free old cache */
    }

    /* Initialise the first four terms (if within bounds) */
    for (idx_t i = cache_size; i < new_size && i < 4; ++i) {
        if (i == 0) new_cache[0] = (seq_t)0;
        if (i == 1) new_cache[1] = (seq_t)10;
        if (i == 2) new_cache[2] = (seq_t)2916;
        if (i == 3) new_cache[3] = (seq_t)933470;
    }

    /* Compute missing terms using the 4‑term recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 4)  /* initial values already set */ 
            continue;
        new_cache[i] = (seq_t)340 * new_cache[i-1]
                     - (seq_t)5798 * new_cache[i-2]
                     + (seq_t)340 * new_cache[i-3]
                     - new_cache[i-4];
    }

    /* Update cache pointers and size */
    cache      = new_cache;
    cache_size = new_size;

    return cache[n];
}
