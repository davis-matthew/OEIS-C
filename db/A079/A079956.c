/*
 * A079956  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={0,1,4}.
 * https://oeis.org/A079956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) + a(n-6) Initial values: a(0)=1, a(1)=0,
 *            a(2)=0
 * Range:     u8 n <= 25, u16 n <= 46, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored */

    /* If we don't yet have enough space, grow the cache */
    if (n >= cache_size) {
        idx_t new_size = n + 1;               /* we need indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed – aborting is acceptable here */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        /* Determine where to start filling in new entries */
        idx_t start_index;
        if (cache_size == 0) {
            /* Initialise the first three elements explicitly */
            cache[0] = 1;
            if (new_size > 1) cache[1] = 0;
            if (new_size > 2) cache[2] = 0;
            start_index = (new_size > 3) ? 3 : new_size;
        } else {
            /* The previous cache already has values up to cache_size-1 */
            start_index = cache_size;
        }

        /* Compute remaining values using the recurrence */
        for (idx_t i = start_index; i <= n; ++i) {
            seq_t term = 0;
            if (i >= 3) term += cache[i - 3];
            if (i >= 4) term += cache[i - 4];
            if (i >= 6) term += cache[i - 6];
            cache[i] = term;
        }

        cache_size = new_size;
    }

    return cache[n];
}
