/*
 * A208215  Number of ways of dividing a 3 X n rectangle into rectangles of
 *          integer side lengths.
 * https://oeis.org/A208215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1,  a(1) = 4,  a(2) = 34,  a(3) = 322
   a(n) = 15*a(n-1) - 55*a(n-2) + 51*a(n-3)   for n >= 4
*/

/* The function uses a memoised cache so that repeated calls with any
 * order of n do not recompute earlier terms.  The cache is stored
 * in static memory and grows with realloc when needed.  All
 * arithmetic is performed with the type seq_t, as required by the
 * build system. */

#include <stdlib.h>

static seq_t  *cache = NULL;          /* cached terms  */
static idx_t   cache_len = 0;         /* number of computed entries */
static size_t  cache_cap = 0;         /* allocated capacity in terms   */

seq_t a(idx_t n)
{
    /* First call – initialise the cache with the 4 base values. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) {
            return (seq_t)0;          /* allocation failed */
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)34;
        cache[3] = (seq_t)322;
        cache_len = 4;
    }

    /* If the requested index is already cached, just return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache if necessary to hold indices up to n. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        size_t target = (size_t)n + 1;          /* at least n+1 entries */
        while (new_cap < target) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            return (seq_t)0;                  /* allocation failed */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 4) {
            /* Base values already set; skip. */
            continue;
        }
        cache[i] = (seq_t)15 * cache[i-1]
                 - (seq_t)55 * cache[i-2]
                 + (seq_t)51 * cache[i-3];
    }

    cache_len = n + 1;   /* we now have terms 0..n stored */
    return cache[n];
}
