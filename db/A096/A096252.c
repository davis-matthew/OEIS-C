/*
 * A096252  Array read by rows, starting with n=0: row n lists
 *          A057077(n+1)*8^(n+1)/2, A057077(n+2)*8^(n+1)/2,
 *          A057077(n+1)*8^(n+1).
 * https://oeis.org/A096252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 61, i128 n <= 125,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096252: a(n) = 4*a(n-2) - 16*a(n-4),  a(0)=4, a(1)=-4, a(2)=8, a(3)=-32 */

#include <stdlib.h>        /* for abort() */
#include <stddef.h>        /* for size_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* cache holds the already computed sequence values, stored in order */
    static seq_t *cache   = NULL;   /* pointer to allocated array   */
    static size_t cache_size = 0;   /* number of terms currently valid */
    static size_t cache_cap  = 0;   /* allocated capacity (elements) */

    /* initialise cache on the first call */
    if (cache_cap == 0) {
        /* we need at least the first four terms */
        cache_cap = 4;
        cache = seq_alloc(cache_cap);
        if (!cache) abort();

        cache[0] = (seq_t)  4;
        cache[1] = (seq_t) -4;
        cache[2] = (seq_t)  8;
        cache[3] = (seq_t)-32;

        cache_size = 4;           /* indices 0..3 are now valid */
    }

    /* enlarge the cache if the requested index is beyond current capacity */
    size_t target = (size_t)n;
    if (target + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= target + 1) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    if (target >= cache_size) {
        for (size_t i = cache_size; i <= target; ++i) {
            /* recurrence: a(i) = 4*a(i-2) - 16*a(i-4) */
            seq_t term = (seq_t)4 * cache[i - 2] - (seq_t)16 * cache[i - 4];
            cache[i] = term;
        }
        cache_size = target + 1;
    }

    return cache[target];
}
