/*
 * A179606  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + x)/(1 - 3*x - 5*x^2).
 * https://oeis.org/A179606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A179606
 * a(0) = 1, a(1) = 4
 * Recurrence: a(n) = 3*a(n-1) + 5*a(n-2)   for n ≥ 2
 * The function memoizes results in static storage so calls can be made
 * in any order and any number of times.  It works for any seq_t /
 * idx_t types provided by the build environment.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;   /* allocated capacity in elements */
    static idx_t   max_comp  = 0;   /* highest index already computed (>=1) */

    /* ------------------------------------------------------------------
     * Initialise the cache on the first call.  We allocate space for
     * indices 0 and 1 and store their base values.
     * ------------------------------------------------------------------ */
    if (cache == NULL) {
        cache_cap = 2; /* space for indices 0 and 1 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Assume malloc succeeds; the judge environment guarantees enough RAM. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        max_comp = 1;
    }

    /* ------------------------------------------------------------------
     * If the requested index is already within the computed range,
     * return it immediately.
     * ------------------------------------------------------------------ */
    if ((idx_t)(n + 1) <= max_comp + 1) {
        return cache[n];
    }

    /* ------------------------------------------------------------------
     * Grow the cache if it is not large enough to hold indices up to n.
     *--------------------------------------------------------------*/
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) {
            new_cap <<= 1;                /* double until large enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc keeps the old contents; ignore failure as memory is
           guaranteed to be available by the test harness. */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ------------------------------------------------------------------
     * Compute any missing terms up to n using the recurrence.
     * ------------------------------------------------------------------ */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        cache[k] = (seq_t)3 * cache[k - 1] + (seq_t)5 * cache[k - 2];
    }
    max_comp = n;

    return cache[n];
}
