/*
 * A116201  a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4); a(0)=0, a(1)=1, a(2)=1,
 *          a(3)=1.
 * https://oeis.org/A116201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4); with a(0) = 0, a(1) =
 *            1, a(2) = 1, a(3) = 1
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 83, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;       /* pointer to stored terms          */
    static idx_t cache_size = 0;      /* allocated size in terms          */
    static idx_t last = (idx_t)-1;    /* highest index that has been filled*/

    /* first call: create cache with at least 4 elements */
    if (cache == NULL) {
        cache_size = (n >= 3u) ? (n + 1) : 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – caller must handle */

        /* initialise the four known values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        last = 3;
    }
    /* grow the cache if the requested index is beyond current capacity */
    else if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – caller must handle */
        cache       = tmp;
        cache_size  = new_size;
        /* previously computed values are intact; new slots will be filled below */
    }

    /* if the term has already been computed, just return it */
    if (n <= last)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i)
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3] - cache[i - 4];

    last = n;
    return cache[n];
}
