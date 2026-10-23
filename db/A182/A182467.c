/*
 * A182467  a(n) = 3*a(n-1) - 2*a(n-2) with a(0)=36 and a(1)=90.
 * https://oeis.org/A182467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 18 (for n >= 2), a(0) = 36, a(1) = 90
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values */
static seq_t *cache   = NULL;   /* array of previously computed terms   */
static idx_t  cache_cap   = 0;   /* allocated size of the cache          */
static idx_t  cache_last  = (idx_t)-1;/* biggest index that has been computed */

/* Ensure that cache contains terms up to index n */
static void ensure_cache(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();              /* memory failure */

        cache[0] = (seq_t)36;
        cache[1] = (seq_t)90;
        cache_last = 1;                    /* we have a(0) and a(1) */
    }

    /* Nothing to do if we already have the required term */
    if (n <= cache_last) return;

    /* Grow cache if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache       = tmp;
        cache_cap   = new_cap;
    }

    /* Compute all missing terms up to n (using the safe recurrence) */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = ((seq_t)2 * cache[i - 1]) + (seq_t)18;
    }
    cache_last = n;
}

/* Return a(n) for any n >= 0, in any call order */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
