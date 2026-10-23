/*
 * A161582  The list of the k values in the common solutions to the 2
 *          equations 5*k+1 = A^2, 9*k+1 = B^2.
 * https://oeis.org/A161582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*a(n-1) - 48*a(n-2) + a(n-3) with a(1) = 0, a(2) = 7,
 *            a(3) = 336
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* o( n ) implementation with memoisation. */
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"
#include <stdbool.h>

/* Static cache holding terms already computed.  */
static seq_t *cache = NULL;          /* 1‑based index: cache[0] unused */
static idx_t  cache_cap = 0;         /* total allocated entries (including 0) */
static idx_t  cached     = 0;        /* highest index computed */

/* Ensure the cache can address indices up to `need` (inclusive). */
static void
ensure_cache(idx_t need)
{
    if (cache == NULL) {
        /* Allocate at least 4 entries (for indices 0..3).  */
        cache_cap = (need + 1 >= 4) ? (need + 1) : 4;
        cache = alloc_seq(cache_cap);
        /* Initialise base values.  */
        cache[0] = 0;            /* unused */
        cache[1] = (seq_t)0;     /* a(1) */
        cache[2] = (seq_t)7;     /* a(2) */
        cache[3] = (seq_t)336;   /* a(3) */
        cached   = 3;
    }
    if (need + 1 > cache_cap) {
        /* Grow cache.  */
        idx_t new_cap = need + 1;
        seq_t *new_cache = alloc_seq(new_cap);
        for (idx_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = cache[i];
        }
        free_seq(cache);
        cache     = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute the nth term of A161582. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* indices < 1 are unused */

    ensure_cache(n);

    /* If the requested term is already computed, return it. */
    if (n <= cached) {
        return cache[n];
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)48 * cache[i-1] - (seq_t)48 * cache[i-2] + cache[i-3];
    }
    cached = n;
    return cache[n];
}
