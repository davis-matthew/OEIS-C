/*
 * A143644  Expansion of 1/(1 - x^3 - x^4 + x^7 - x^10 - x^11 + x^14) (a Salem
 *          polynomial).
 * https://oeis.org/A143644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 68, u32 n <= 128, u64 n <= 250,
 *            u128 n <= 493, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143644
   a(n) = a(n-3) + a(n-4) - a(n-7) + a(n-10) + a(n-11) - a(n-14)
   negative indices are treated as 0
   initial values: a(0)=1, a(1)=0, a(2)=0 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;   /* array of computed terms */
static idx_t  cache_len = 0;  /* next free index in cache */
static size_t cache_cap = 0;  /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;                                      /* start size */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                          /* unlikely failure */
        /* zero initialise the cache */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;                                   /* a(0) */
        cache[1] = (seq_t)0;                                   /* a(1) */
        cache[2] = (seq_t)0;                                   /* a(2) */
        cache_len = 3;
    }

    /* if the requested term is already in the cache, return it */
    if (n < cache_len) return cache[n];

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)0;
        if (i >= 3)  val += cache[i - 3];  /* + a(i-3) */
        if (i >= 4)  val += cache[i - 4];  /* + a(i-4) */
        if (i >= 10) val += cache[i -10];  /* + a(i-10) */
        if (i >= 11) val += cache[i -11];  /* + a(i-11) */
        if (i >= 7)  val -= cache[i - 7];  /* - a(i-7) */
        if (i >= 14) val -= cache[i -14];  /* - a(i-14) */
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
