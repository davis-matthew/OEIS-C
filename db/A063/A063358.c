/*
 * A063358  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          85 ).
 * https://oeis.org/A063358
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 * Initial values
 *            (for n = 2 … 13) are hard‑coded; values for n < 2 are * treated
 *            as 0. The function memoises computed terms in a static * array,
 *            extending it as larger n are requested
 * Range:     i8 n <= 2, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* Static cache that grows on demand.  cache[0] and cache[1] are dummy. */
    static seq_t *cache = NULL;      /* pointer to cached values   */
    static idx_t  cache_size = 0;    /* current number of stored terms */

    if (n < 2)           /* harmless fallback for n < 2 */
        return 0;

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        /* 14 slots: indices 0 … 13 inclusive. */
        cache_size = 14;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failed */

        /* dummy values for indices 0 and 1 */
        cache[0] = 0;
        cache[1] = 0;

        /* hard‑coded initial terms (indices start at 2). */
        cache[2]  = -1;
        cache[3]  = 215;
        cache[4]  = 480;
        cache[5]  = 742;
        cache[6]  = 1004;
        cache[7]  = 1266;
        cache[8]  = 1528;
        cache[9]  = 1790;
        cache[10] = 2052;
        cache[11] = 2314;
        cache[12] = 2576;
        cache[13] = 2842;
    }

    /* If requested n is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to include up to n. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) abort();          /* allocation failed */
    cache = tmp;

    /* Fill in any missing values using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i > 13) {
            /* indices i-4, i-6, i-10 are guaranteed to exist */
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
        } else {
            /* for i <= 13 the value is already known from the
               initialisation; nothing to do. */
        }
    }

    cache_size = new_size;
    return cache[n];
}
