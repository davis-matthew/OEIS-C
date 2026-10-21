/*
 * A087826  a(n) = a(n-a(n-1)) + a(a(n-a(n-2))).
 * https://oeis.org/A087826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 877, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087826: a(n) = a(n-a(n-1)) + a(a(n-a(n-2)))  (offset 1) */
#include <stdlib.h>
#include <string.h>

/* cache[0] is unused; cache[1] and cache[2] are the base cases (1,1) */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Ensure the cache can hold values up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    /* Need indices 0 .. n, so allocate n+1 elements. */
    idx_t needed = n + 1;
    if (needed <= cache_size)
        return;

    seq_t *tmp = (seq_t *)malloc(needed * sizeof(seq_t));
    if (!tmp)
        return;               /* Allocation failure – treat as empty. */

    if (cache) {
        memcpy(tmp, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    /* Zero the newly allocated part. */
    memset(tmp + cache_size, 0, (needed - cache_size) * sizeof(seq_t));
    cache = tmp;
    cache_size = needed;

    /* Initialise the two base terms when they are not yet set. */
    if (cache_size >= 2 && cache[1] == 0)
        cache[1] = (seq_t)1;
    if (cache_size >= 3 && cache[2] == 0)
        cache[2] = (seq_t)1;
}

/* Return a(n).  The function works for arbitrary-precision seq_t
 * and for integer seq_t types, and handles calls in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* Undefined for n < 1 */

    /* Base values.  They are not stored in the cache to avoid
     * special‑case logic during construction. */
    if (n <= 2)
        return (seq_t)1;

    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0)          /* Already computed. */
            continue;

        /* Compute a(i-a(i-1)). */
        idx_t t1 = i - (idx_t)cache[i - 1];
        seq_t a_t1 = cache[t1];

        /* Compute a(a(i-a(i-2))). */
        idx_t t2   = i - (idx_t)cache[i - 2];
        idx_t inner_index = (idx_t)cache[t2];
        seq_t a_t2 = cache[inner_index];

        cache[i] = a_t1 + a_t2;
    }

    return cache[n];
}
