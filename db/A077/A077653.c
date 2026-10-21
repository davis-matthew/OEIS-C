/*
 * A077653  a(1)=1, a(2)=2, a(3)=2, a(n) = abs(a(n-1) - a(n-2) - a(n-3)).
 * https://oeis.org/A077653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 2, a(n) = abs(a(n-1) - a(n-2) -
 *            a(n-3)) for n >= 4 The function is memoised and works for calls
 *            in any order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;   /* cache[n] = a(n);  cache[0] unused */
static idx_t  cache_size  = 0;  /* allocated elements (indexes 0..cache_size-1) */
static idx_t  cache_last  = 0;  /* highest n for which cache[n] is valid */

/* Ensure cache is large enough to address index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = cache_size ? cache_size : 16;
    while (new_size <= n) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory */
    cache = tmp;
    cache_size = new_size;
}

/* Compute a(n) for n >= 1. Returns 0 for n <= 0. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined below the offset */

    /* Initialise base terms on first use. */
    if (cache_last == 0) {
        ensure_cache(3);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache_last = 3;
    }

    /* If already computed, just return. */
    if (n <= cache_last) return cache[n];

    /* Grow storage if necessary. */
    ensure_cache(n);

    /* Iteratively compute missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* a(i) = abs(a(i-1) - a(i-2) - a(i-3)) */
        signed __int128 diff = (signed __int128)cache[i-1]
                                 - (signed __int128)cache[i-2]
                                 - (signed __int128)cache[i-3];
        if (diff < 0) diff = -diff;
        cache[i] = (seq_t)diff;
    }

    cache_last = n;
    return cache[n];
}
