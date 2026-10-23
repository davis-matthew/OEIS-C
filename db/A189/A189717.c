/*
 * A189717  Partial sums of A156595.
 * https://oeis.org/A189717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)/3) + floor(n/3) - a(floor(n/3)), a(0)=0
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoised values, cache[0] = a(0) */
static size_t cache_cap  = 0; /* allocated capacity */
static size_t cache_next = 0; /* next free index (cache_next == number of stored terms) */

seq_t a(idx_t n)
{
    if (n == 0)             /* safety for internal use */
        return (seq_t)0;

    /* initialise cache to hold a(0) */
    if (cache_next == 0) {
        size_t init_cap = 8;                              /* start with a small buffer */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;                              /* a(0) = 0 */
        cache_cap  = init_cap;
        cache_next = 1;                                     /* next free index is 1 */
    }

    /* grow cache if needed */
    if ((idx_t)cache_next <= n) {
        /* ensure enough capacity for indices up to n */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap = new_cap == 0 ? 8 : new_cap * 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;

        /* compute missing terms from cache_next to n inclusive */
        idx_t start = (idx_t)cache_next;
        for (idx_t i = start; i <= n; ++i) {
            seq_t t1 = (seq_t)((i + 1) / 3);      /* floor((i+1)/3) */
            seq_t t2 = (seq_t)(i / 3);            /* floor(i/3)     */
            seq_t sub = cache[i / 3];             /* a(floor(i/3)) */
            cache[i] = t1 + t2 - sub;             /* recurrence */
        }
        cache_next = n + 1;                        /* all terms up to n are stored */
    }

    return cache[n];
}
