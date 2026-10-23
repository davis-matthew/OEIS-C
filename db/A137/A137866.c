/*
 * A137866  a(1)=0. For n >= 2, a(n) = gcd(a(n-1)+1, n).
 * https://oeis.org/A137866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 443, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A137866
   Formula: a(1) = 0;
            for n ≥ 2, a(n) = gcd(a(n‑1) + 1, n)
*/

#include "../utils/gcd.h"
#include <stddef.h>
#include <stdlib.h>

/* Cached values, 1‑based indexing.  cache[0] is unused. */
static seq_t *cache = NULL;        /* 1‑based cache pointer         */
static size_t cache_cap = 0;        /* allocated size (bytes of seq_t)*/
static idx_t  cache_next = 0;      /* next index to compute (i.e. cache_next-1 is last computed) */

/* Ensure that cache contains values up to and including index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_next)          /* already computed */
        return;

    /* Grow cache storage if necessary. */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            abort();            /* out of memory */
        cache = tmp;
        cache_cap = needed;
    }

    /* Compute missing terms.  cache_next is the first index
       that has NOT yet been computed.  After the loop,
       cache_next will be n + 1. */
    for (idx_t i = cache_next; i <= n; ++i) {
        seq_t prev_plus = cache[i - 1] + (seq_t)1;
        cache[i] = gcd(prev_plus, (seq_t)i);
    }
    cache_next = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for n = 0, return 0 */
        return (seq_t)0;

    /* Lazy initialization of the cache: cache[1] = 0. */
    if (!cache) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));  /* indices 0 and 1 */
        if (!cache)
            abort();            /* out of memory */
        cache[0] = (seq_t)0;    /* unused slot, kept zero */
        cache[1] = (seq_t)0;    /* a(1) = 0 */
        cache_cap = 2;
        cache_next = 2;         /* next index to compute */
    }

    ensure_cache(n);
    return cache[n];
}
