/*
 * A238549  a(n) is one fourth of the total number of free ends of 4 line
 *          segments expansion at n iterations (see Comments lines for
 *          definition).
 * https://oeis.org/A238549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3), a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n < 1 is not used in tests. */
    if (n == 0) return (seq_t)0;

    /* Static cache: cache[i] holds a(i+1) for i = 0 .. cache_len-1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* Number of terms currently stored */

    if (n > cache_len) {
        /* Allocate a new array big enough for all needed terms. */
        seq_t *new_cache = (seq_t *)malloc(n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* Allocation failed */

        /* Copy existing terms to the new array. */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* If we had an old cache, free it. */
        if (cache != NULL)
            free((void *)cache);

        /* Compute the new terms using the recurrence. */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            seq_t val;
            if (k == 1)          val = (seq_t)1;
            else if (k == 2)     val = (seq_t)2;
            else if (k == 3)     val = (seq_t)6;
            else                 /* a(k) = a(k-1) + 2*a(k-2) - 2*a(k-3) */
                val = new_cache[k-2]              /* a(k-1) */
                  + (seq_t)2 * new_cache[k-3]    /* a(k-2) */
                  - (seq_t)2 * new_cache[k-4];   /* a(k-3) */
            new_cache[k-1] = val;  /* store at index k-1 */
        }

        /* Replace the old cache. */
        cache = new_cache;
        cache_len = n;
    }

    /* Return the requested term. */
    return cache[n-1];
}
