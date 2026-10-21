/*
 * A076271  a(1) = 1, a(2) = 2, and for n > 2, a(n) = a(n-1) + gpf(a(n-1)),
 *          where gpf = greatest prime factor = A006530.
 * https://oeis.org/A076271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + gpf(a(n-1)), with a(1)=1, a(2)=2
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* gpf = greatest prime factor (A006530) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gpf.h"

/* Static cache: cache[0] = a(1), cache[1] = a(2), ... */
static seq_t *cache       = NULL; /* pointer to cached values          */
static size_t cached      = 0;    /* number of computed terms stored   */
static size_t cache_cap   = 0;    /* allocated cache capacity          */

/* resize the cache if needed                                            */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)    /* allocation error, abort safely */
        exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

/* sequence a(n) ============================================================== */
seq_t a(idx_t n)
{
    /* not expected for n==0, return 0 */
    if (n == 0)
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        ensure_cache_cap(4);
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cached   = 2;
    }

    /* already computed ? */
    if ((size_t)n <= cached)
        return cache[n - 1];

    /* compute missing terms up to n */
    ensure_cache_cap((size_t)n);

    for (size_t i = cached; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        /* use the helper to obtain the greatest prime factor; cast to
           uint64_t is safe for all values that fit in seq_t (and the tests
           stay far below the 64‑bit overflow threshold)                     */
        uint64_t prev_ui = (uint64_t)prev;
        uint64_t g       = gpf(prev_ui);
        cache[i] = prev + (seq_t)g;
    }

    cached = (size_t)n;
    return cache[n - 1];
}
