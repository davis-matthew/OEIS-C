/*
 * A141015  a(0) = 0, a(1) = 1, a(2) = 2; for n > 2, a(n) = a(n-1) + 2*a(n-2)
 *          + a(n-3).
 * https://oeis.org/A141015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 2; a(n) = a(n-1) + 2*a(n-2) + a(n-3)
 *            for n > 2 (OEIS A141015)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;        /* array holding computed a(n) values      */
static size_t cache_cap = 0;       /* allocated size of cache                */
static size_t cache_len = 0;       /* number of values actually stored      */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary.  We keep it at least n+1 elements. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n + 1;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed: terminate the program */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)2;
        } else {
            cache[(size_t)i] = cache[(size_t)(i - 1)]
                              + 2 * cache[(size_t)(i - 2)]
                              + cache[(size_t)(i - 3)];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
