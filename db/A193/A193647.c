/*
 * A193647  Number of arrays of -7..7 integers x(1..n) with every x(i) in a
 *          subsequence of length 1 or 2 with sum zero.
 * https://oeis.org/A193647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 12*a(n-2) + a(n-3) Initial values: a(1)=1,
 *            a(2)=15, a(3)=43 Computed by the empirical linear recurrence
 *            (OEIS A193647)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 0 is undefined in the sequence; return 0 to be safe */
    if (n == 0) return 0;

    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current allocated size (index 0..cap-1) */

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t old_cap  = cap;               /* old size */
        size_t new_cap  = (size_t)n + 1;     /* need indices 0..n */

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failed */
        cache = new_cache;
        cap   = new_cap;

        /* Initialise the new entries */
        for (idx_t i = (idx_t)old_cap; i <= n; ++i) {
            if (i == 1)                 cache[i] = 1;
            else if (i == 2)            cache[i] = 15;
            else if (i == 3)            cache[i] = 43;
            else                        cache[i] = ((seq_t)2) * cache[i-1]
                                          + ((seq_t)12) * cache[i-2]
                                          + cache[i-3];
        }
    }

    return cache[n];
}
