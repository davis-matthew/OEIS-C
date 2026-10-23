/*
 * A027558  Number of 3-unbalanced strings of length n (= 2^n - A027557(n)).
 * https://oeis.org/A027558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * A005674(n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, abort */
#include <string.h>   /* memcpy */

seq_t a(idx_t n)
{
    /* Cache of the half-values A005674(n).  cache[i] = A005674(i). */
    static seq_t *cache = NULL;
    static idx_t  size  = 0;   /* number of valid entries in cache */

    /* Grow the cache if necessary. */
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            abort();            /* allocation failure */

        /* Copy old entries if any. */
        if (size > 0) {
            memcpy(new_cache, cache, size * sizeof(seq_t));
            free(cache);
        }

        /* If starting from scratch, initialise the first five terms. */
        if (size == 0) {
            if (new_size > 0) new_cache[0] = (seq_t)0;
            if (new_size > 1) new_cache[1] = (seq_t)0;
            if (new_size > 2) new_cache[2] = (seq_t)0;
            if (new_size > 3) new_cache[3] = (seq_t)0;
            if (new_size > 4) new_cache[4] = (seq_t)1;
        }
        /* For indices that were previously missing (<5), set them
           (this happens when the cache grows from size<5). */
        else if (size < 5) {
            idx_t limit = new_size < 5 ? new_size : 5;
            for (idx_t i = size; i < limit; ++i) {
                new_cache[i] = (i == 4) ? (seq_t)1 : (seq_t)0;
            }
        }

        /* Compute new entries using the recurrence.
           Recurrence holds for i >= 5:
           b(i) = 3*b(i-1) + b(i-2) - 8*b(i-3) + 2*b(i-4) + 4*b(i-5). */
        idx_t start = (size > 5 ? size : 5);
        for (idx_t i = start; i < new_size; ++i) {
            seq_t sum = 3 * new_cache[i-1]
                      +     new_cache[i-2]
                      + 2 * new_cache[i-4]
                      + 4 * new_cache[i-5];
            new_cache[i] = sum - 8 * new_cache[i-3];
        }

        cache = new_cache;
        size  = new_size;
    }

    /* cache[n] now holds A005674(n).  Multiply by 2 for a(n). */
    return (seq_t)2 * cache[n];
}
