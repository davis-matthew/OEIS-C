/*
 * A221536  Number of 0..2 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 4*a(n-4) for n>=5; Initial values:
 *            a(1)=0, a(2)=2, a(3)=4, a(4)=10
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values (zero‑based index). */
static seq_t *cache = NULL;
static idx_t cached_max = 0;

/* Return the number of 0..2 arrays of length n with the required property.
   Results are memoised; each n≥1 is computed in O(n) time cumulatively. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Guard: undefined for n=0 */

    if (n > cached_max) {
        /* Allocate a larger array, preserving previous values. */
        seq_t *new_cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!new_cache) abort();   /* Allocation failed */

        for (idx_t i = 0; i < cached_max; ++i)
            new_cache[i] = cache[i];

        /* Compute new terms. */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)
                val = 0;
            else if (i == 2)
                val = 2;
            else if (i == 3)
                val = 4;
            else if (i == 4)
                val = 10;
            else
                val = (seq_t)3 * new_cache[i - 2]
                    - (seq_t)2 * new_cache[i - 3]
                    + (seq_t)4 * new_cache[i - 5];
            new_cache[i - 1] = val;
        }

        free(cache);
        cache = new_cache;
        cached_max = n;
    }

    return cache[n - 1];
}
