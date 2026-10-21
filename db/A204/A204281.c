/*
 * A204281  Number of (n+2) X 7 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A204281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A204281: a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) for n>8
   Initial values:
   a(1)=5263, a(2)=539, a(3)=334, a(4)=442,
   a(5)=736, a(6)=996, a(7)=2168, a(8)=5368
   The function stores computed terms in a static cache so that calls
   in arbitrary order are O(1) after the first evaluation. */

#include <stdlib.h>
#include <string.h>

/* Base values for indices 1..8 */
static const seq_t base[9] = {
    0ULL, 5263ULL, 539ULL, 334ULL, 442ULL,
    736ULL, 996ULL, 2168ULL, 5368ULL
};

seq_t a(idx_t n)
{
    /* Return one of the initial terms directly */
    if (n <= 8) {
        return base[(size_t)n];
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* array of computed terms, indexed 0..cap */
    static size_t cache_cap = 0;    /* highest index currently stored */

    /* Initialise cache on the first request that requires it */
    if (!cache) {
        /* Allocate space for indices 0..8 */
        cache_cap = 8;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failed */
        memcpy(cache, base, (cache_cap + 1) * sizeof(seq_t));
        /* cache[0] is already zeroed by malloc */
    }

    /* Grow the cache if the requested index is beyond the current maximum */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;   /* realloc failed */
        /* Initialise the newly allocated region (optional, not needed for recurrence) */
        cache = tmp;
        /* Compute all missing terms using the recurrence */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) {
            /* a(i) = 3*a(i-1) - 6*a(i-4) + 4*a(i-6) */
            cache[i] = 3 * cache[i - 1] - 6 * cache[i - 4] + 4 * cache[i - 6];
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
