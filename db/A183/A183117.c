/*
 * A183117  Magnetic Tower of Hanoi, number of moves of disk number k,
 *          optimally solving the [NEUTRAL ; NEUTRAL ; NEUTRAL] pre-colored
 *          puzzle.
 * https://oeis.org/A183117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3) - 6*a(n-4) for n>5 Initial
 *            values: a(0)=0, a(1)=1, a(2)=3, a(3)=7, a(4)=19, a(5)=53
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t last = (idx_t)-1;   /* last computed index */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* we need at least indices 0..5 for the recurrence */
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed, return 0 */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 7;
        cache[4] = 19;
        cache[5] = 53;
        last = 5;
    }

    /* if n is already computed, just return it */
    if (n <= last)
        return cache[n];

    /* extend cache up to n */
    idx_t new_size = n + 1;
    if (new_size <= last + 1) return cache[n];  /* nothing to do */

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failed; keep the old cache and return last known value */
        return cache[last];
    }
    cache = new_cache;

    /* compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 5) {
            /* base values already set, but guard in case of small n */
            switch (i) {
            case 0: cache[i] = 0; break;
            case 1: cache[i] = 1; break;
            case 2: cache[i] = 3; break;
            case 3: cache[i] = 7; break;
            case 4: cache[i] = 19; break;
            case 5: cache[i] = 53; break;
            }
        } else {
            cache[i] = 3 * cache[i - 1] + cache[i - 2]
                     - cache[i - 3] - 6 * cache[i - 4];
        }
    }

    last = n;
    return cache[n];
}
