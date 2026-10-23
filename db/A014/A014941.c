/*
 * A014941  a(1)=1, a(n) = n*23^(n-1) + a(n-1).
 * https://oeis.org/A014941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*23^(n-1) + a(n-1), a(1) = 1 (equivalently, a(n) =
 *            (23^n*(22*n-1)+1)/484.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h> /* for memcpy */

/* The types seq_t and idx_t are defined by the build system.  */
seq_t a(idx_t n)
{
    /* Return 0 for indices before the offset (undefined behaviour otherwise). */
    if (n == 0) return 0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;   /* allocated array of terms */
    static size_t cache_size = 0; /* number of terms currently stored (>=0) */
    static size_t cache_cap  = 0; /* allocated capacity, in terms of terms */

    /* Static power of 23: 23^(current_index).  
       When cache_size == k, pow23 == 23^k. */
    static seq_t pow23 = 1;

    /* Expand the cache if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;                     /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t term = ((seq_t)i) * pow23;           /* i * 23^(i-1) */
        if (i == 1) {
            cache[0] = term;                      /* a(1) = 1 */
        } else {
            cache[(size_t)i - 1] = cache[(size_t)i - 2] + term;
        }
        pow23 = pow23 * ((seq_t)23);              /* prepare for next i */
    }

    cache_size = (size_t)n; /* update stored size */
    return cache[(size_t)n - 1];
}
