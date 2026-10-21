/*
 * A070867  a(1) = a(2) = 1; a(n) = a(n-1 - a(n-1)) + a(n-1 - a(n-2)).
 * https://oeis.org/A070867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1; a(n) = a(n-1 - a(n-1)) + a(n-1 - a(n-2)), for n
 *            ≥ 3
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoised values.
 * cache[0] is unused; valid indices are 1 ... cache_cap. */
static seq_t *cache = NULL;
static idx_t   cache_cap = 0;   /* highest allocated index (cache_cap ≥ 0) */
static idx_t   cache_last = 0;  /* highest index that has been computed */

/* Ensure the cache can hold indices up to `limit`.  All new entries
 * are initialised to 0.  The old data is copied unchanged. */
static void ensure_capacity(idx_t limit)
{
    if (limit <= cache_cap)
        return;

    /* Allocate a new array large enough for indices 0 … limit. */
    seq_t *new_cache = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);                     /* fatal – keep the build simple */

    /* Zero‑initialise the new array. */
    for (idx_t i = 0; i <= limit; ++i)
        new_cache[i] = (seq_t)0;

    /* Copy any existing values. */
    if (cache) {
        for (idx_t i = 0; i <= cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache     = new_cache;
    cache_cap = limit;
}

/* Compute the sequence up to and including index `n`. */
static void compute_up_to(idx_t n)
{
    if (n < 1)
        return;                       /* undefined for n < 1 */

    /* Base cases must be available even if `n` is 1 or 2. */
    idx_t need = n;
    if (need < 2) need = 2;
    ensure_capacity(need);

    /* Initialise the two known values (overwriting is harmless). */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;

    if (n <= 2) {
        cache_last = n;
        return;
    }

    /* Start computing from the next uncomputed index. */
    idx_t start = (cache_last >= 3) ? cache_last + 1 : 3;
    for (idx_t i = start; i <= n; ++i) {
        seq_t a_im1 = cache[i - 1];   /* a(n-1)    */
        seq_t a_im2 = cache[i - 2];   /* a(n-2)    */

        idx_t idx1 = (idx_t)i - 1 - (idx_t)a_im1;
        idx_t idx2 = (idx_t)i - 1 - (idx_t)a_im2;

        seq_t t1 = cache[idx1];
        seq_t t2 = cache[idx2];

        cache[i] = t1 + t2;
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1; return 0 as a safe placeholder */
        return (seq_t)0;

    compute_up_to(n);
    return cache[n];
}
