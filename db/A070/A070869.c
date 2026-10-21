/*
 * A070869  a(1) = 16; a(n+1) = sum of a(n) and (a(n) written in base 2 and
 *          reversed).
 * https://oeis.org/A070869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   rev2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=16; a(n+1)=a(n)+rev2(a(n))   */
/* rev2(x) = binary representation of x reversed          */
#include <stdlib.h>
#include "../utils/rev2.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;     /* number of allocated entries (0..cache_size-1)   */
static idx_t max_computed = 0;   /* highest index that has been filled             */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;        /* values below the offset are irrelevant */

    /* initialise the cache on the first call */
    if (!cache) {
        cache_size   = 2;        /* indices 0 and 1 */
        cache        = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0]     = (seq_t)0; /* unused but convenient for formulas      */
        cache[1]     = (seq_t)16;
        max_computed = 1;
    }

    /* extend the cache if necessary */
    if (n >= cache_size) {
        idx_t newsize = n + 1;   /* include index n                       */
        seq_t *newcache = (seq_t *)malloc(newsize * sizeof(seq_t));
        if (!newcache) return (seq_t)0;
        for (idx_t i = 0; i < cache_size; ++i)
            newcache[i] = cache[i];
        free(cache);
        cache        = newcache;
        cache_size   = newsize;
    }

    /* compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i]   = prev + rev2(prev);
    }
    max_computed = n;
    return cache[n];
}
