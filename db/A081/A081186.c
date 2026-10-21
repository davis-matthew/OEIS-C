/*
 * A081186  4th binomial transform of (1,0,1,0,1,...), A059841.
 * https://oeis.org/A081186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 15*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: per‑call memoisation of all previously computed terms. */
    static seq_t *cache   = NULL;
    static size_t  cache_cap   = 0;   /* allocated capacity of 'cache'        */
    static idx_t   max_computed = 0;   /* highest index already computed      */

    /* ------------------------------------------------------------------
       Initialise the cache (and the two initial values) the first time
       the function is called.  Subsequent calls skip this block.                */
    if (cache_cap < 2) {
        size_t new_cap = 2;
        if (cache_cap > 0) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
        } else {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) return 0;
        }
        cache_cap = new_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        max_computed = 1;                     /* indices 0 and 1 are ready     */
    }

    /* ------------------------------------------------------------------
       If the requested index has already been filled the answer is
       returned immediately.                                              */
    if (n <= max_computed)
        return cache[n];

    /* ------------------------------------------------------------------
       Ensure there is enough room for the new terms.  The cache is
       grown geometrically (double each time), which gives amortised
       O(1) work for insertion.                                            */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ------------------------------------------------------------------
       Compute every missing term up to n, using the recurrence.          */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The first two terms are already in the array, so this branch
           only becomes relevant when i reaches 2 or higher.                */
        cache[i] = (seq_t)(8 * cache[i - 1] - 15 * cache[i - 2]);
    }

    max_computed = n;
    return cache[n];
}
