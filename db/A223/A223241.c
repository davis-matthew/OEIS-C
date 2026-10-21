/*
 * A223241  3-loop graph coloring a rectangular array: number of n X 2 0..6
 *          arrays where 0..6 label nodes of a graph with edges 0,1 1,2 2,0
 *          0,3 3,4 4,0 0,5 5,6 6,0 and every array movement to a horizontal
 *          or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 4*a(n-2), a(1)=18, a(2)=102
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), ... */
static size_t cache_cap = 0;  /* number of entries currently stored */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1 */

    size_t needed = (size_t)n;          /* number of terms that must be present */

    /* Expand cache if necessary */
    if (cache_cap < needed) {
        seq_t *newc = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!newc)
            return 0;        /* allocation failure → zero fallback */
        cache = newc;

        /* Initialize missing entries */
        if (cache_cap == 0) {               /* first allocation */
            if (needed >= 1)
                cache[0] = (seq_t)18;
            if (needed >= 2)
                cache[1] = (seq_t)102;
        } else if (cache_cap == 1) {        /* extend from a(1) to a(2) */
            if (needed >= 2)
                cache[1] = (seq_t)102;
        }

        /* Compute new terms using the recurrence */
        size_t start;
        if (cache_cap <= 1)
            start = 2;                     /* we already set a(1) and a(2) */
        else
            start = cache_cap;             /* terms up to cache_cap-1 already known */

        for (size_t i = start; i < needed; ++i) {
            cache[i] = 7 * cache[i-1] - 4 * cache[i-2];
        }

        cache_cap = needed;
    }

    return cache[n-1];
}
