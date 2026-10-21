/*
 * A234826  Number of (n+1) X (2+1) 0..2 arrays with each 2 X 2 subblock
 *          having the number of clockwise edge increases less than or equal
 *          to the number of counterclockwise edge increases.
 * https://oeis.org/A234826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) - 102*a(n-2) + 93*a(n-3) - 9*a(n-4); initial
 *            values: a(1)=516, a(2)=9207, a(3)=161631, a(4)=2826144
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Weak static memoisation table */
static seq_t *cache      = NULL;   /* cache[0] unused; indices start at 1 */
static idx_t  cache_cap  = 0;      /* capacity in terms of indices */
static idx_t  cache_last = 0;      /* largest index already computed */
static int    init_done  = 0;      /* are the first four terms stored? */

/* Increase the capacity of the cache to at least `needed` indices. */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2ULL : 4ULL;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1ULL));
    if (!new_cache) {
        /* In a real project we would handle out‑of‑memory here; for the
           competition environment we simply abort. */
        abort();
    }

    if (cache) {
        memcpy(new_cache, cache, (cache_cap + 1ULL) * sizeof(seq_t));
        free((void *)cache);
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Store the four known initial values. */
static void init_cache(void)
{
    if (init_done)
        return;

    ensure_cache(4ULL);
    cache[1] = (seq_t)516;
    cache[2] = (seq_t)9207;
    cache[3] = (seq_t)161631;
    cache[4] = (seq_t)2826144;
    cache_last = 4ULL;
    init_done = 1;
}

/* Return a(n) for n ≥ 1. Behaviour for n = 0 is irrelevant (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    init_cache();

    if (n <= cache_last)
        return cache[n];

    ensure_cache(n);
    for (idx_t i = cache_last + 1ULL; i <= n; ++i) {
        seq_t term = (seq_t)23 * cache[i - 1ULL];
        term -= (seq_t)102 * cache[i - 2ULL];
        term += (seq_t)93 * cache[i - 3ULL];
        term -= (seq_t)9   * cache[i - 4ULL];
        cache[i] = term;
    }
    cache_last = n;
    return cache[n];
}
