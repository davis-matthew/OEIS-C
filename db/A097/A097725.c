/*
 * A097725  Chebyshev U(n,x) polynomial evaluated at x=51.
 * https://oeis.org/A097725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 102*a(n-1) - a(n-2), n ≥ 1, * a(0) = 1, a(-1) = 0 (used
 *            in the recurrence)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached terms so that each term is computed at most once. */
static seq_t   *a_cache   = NULL;     /* array of computed a(n) values */
static size_t   cache_len = 0;        /* current number of cached terms */

/* Return the n‑th term of A097725. */
seq_t a(idx_t n)
{
    /* n is unsigned, but we need a size_t index for realloc.  Tests never
     * exceed SIZE_MAX, so a cast is safe. */
    size_t i, limit = (size_t)n;

    /* Ensure cache is large enough.  We grow it incrementally to avoid
     * repeated reallocs.  The first two terms are initialized once.
     */
    if (cache_len == 0) {
        /* Allocate space for the first two terms. */
        a_cache = (seq_t *)realloc(a_cache, 2 * sizeof(seq_t));
        a_cache[0] = 1;           /* a(0)   = 1 */
        a_cache[1] = 102;         /* a(1)   = 102*1 - a(-1) = 102 */
        cache_len = 2;
    }

    /* Extend the cache up to the requested index, if necessary. */
    if (limit + 1 > cache_len) {
        a_cache = (seq_t *)realloc(a_cache, (limit + 1) * sizeof(seq_t));
        /* Compute new terms iteratively.  For i ≥ 2 we use the recurrence. */
        for (i = cache_len; i <= limit; ++i) {
            if (i == 0) {
                a_cache[i] = 1;
            } else if (i == 1) {
                a_cache[i] = 102;
            } else {
                /* a(i) = 102 * a(i-1) - a(i-2) */
                seq_t term = a_cache[i-1] * 102;
                a_cache[i] = term - a_cache[i-2];
            }
        }
        cache_len = limit + 1;
    }

    return a_cache[limit];
}
