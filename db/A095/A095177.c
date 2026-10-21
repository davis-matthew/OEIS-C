/*
 * A095177  E.g.f.: exp(x)/(1-x)^5.
 * https://oeis.org/A095177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+5)*a(n-1) - (n-1)*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Computes A095177 term a(n).  Supports any seq_t and idx_t types
   provided by the build. */
seq_t a(idx_t n)
{
    /* Ensure base values are present */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)6;          /* a(1) */
    } else if (cache_size == 1) {
        cache = (seq_t*)realloc(cache, 2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache_size = 2;
    } else if (cache_size == 0) {
        /* should never happen, but just in case */
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
    }

    /* If requested index already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* Extend cache to accommodate index n */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));

    /* Compute missing terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        /* For i=0 or 1 the loop is skipped */
        seq_t term = ((seq_t)(i + 5)) * cache[i - 1]
                   - ((seq_t)(i - 1)) * cache[i - 2];
        cache[i] = term;
    }

    return cache[n];
}
