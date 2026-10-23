/*
 * A147841  a(n) = 11*a(n-1) - 9*a(n-2) with a(0)=1, a(1)=9.
 * https://oeis.org/A147841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 9; a(n) = 11*a(n-1) - 9*a(n-2) for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;       /* cached sequence values */
static idx_t  cache_len = 0;      /* allocated size of cache (elements) */
static idx_t  last_computed = 0;  /* index of the largest computed value */
static bool   init       = false; /* has the cache been initialized? */

/* Ensure that cache contains values up to index n (inclusive) */
static void _ensure_cache(idx_t n)
{
    /* Grow the cache if necessary */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();             /* out of memory – not expected */
        cache = tmp;
        cache_len = new_len;
    }

    /* Initialise base cases if this is the first time */
    if (!init) {
        cache[0] = (seq_t)1;
        if (n >= 1) {
            cache[1] = (seq_t)9;
            last_computed = 1;
        } else {
            last_computed = 0;
        }
        init = true;
    }

    /* Compute any missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = ((seq_t)11 * cache[i-1]) - ((seq_t)9 * cache[i-2]);
    }
    if (n > last_computed) last_computed = n;
}

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* handle the very first term directly */
    _ensure_cache(n);
    return cache[n];
}
