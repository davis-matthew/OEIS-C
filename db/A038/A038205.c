/*
 * A038205  Number of derangements of n where minimal cycle size is at least
 *          3.
 * https://oeis.org/A038205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*a(n-1) + (n-1)*(n-2)*a(n-3); initial values:
 *            a(0)=1, a(1)=0, a(2)=0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* memoised values */
    static size_t  cap    = 0;      /* allocated capacity */
    static idx_t  max_cached = 0;   /* highest index computed */
    static int    initialized = 0;

    /* First-time initialisation: give a small capacity that covers at least
       the first three indices (0,1,2).  We double the capacity while it
       is insufficient for the largest required index. */
    if (!initialized) {
        size_t needed = 4;                     /* default minimum size */
        if ((size_t)n + 1 > needed) {
            needed = (size_t)n + 1;
        }
        size_t new_cap = 1;
        while (new_cap < needed)
            new_cap <<= 1;

        cache = (seq_t*)realloc(NULL, new_cap * sizeof(seq_t));
        if (!cache)                         /* allocation failure */
            return (seq_t)0;

        cap = new_cap;
        /* initialise the base cases */
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)0;
        if (cap > 2) cache[2] = (seq_t)0;

        /* max_cached is the highest index we have filled so far */
        max_cached = (cap > 2 ? 2 : (cap > 1 ? 1 : 0));
        initialized = 1;
    }

    /* If the desired index is already computed, just return it. */
    if (n <= max_cached)
        return cache[n];

    /* Ensure the buffer can hold values up to index n. */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = (cap > 0 ? cap : 1);
        while (new_cap < needed)
            new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                         /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values from max_cached+1 up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* The first three values are already set. */
        if (i >= 3) {
            seq_t part1 = ((seq_t)(i - 1)) * cache[i - 1];
            seq_t part2 = ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * cache[i - 3];
            cache[i] = part1 + part2;
        }
    }
    max_cached = n;

    return cache[n];
}
