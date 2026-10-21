/*
 * A090792  a(0)=1; for n>0, a(n)=a([n/2])+a([n/4])+a([n/8]).
 * https://oeis.org/A090792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a([n/2]) + a([n/4]) + a([n/8]), a(0) = 1
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises the values in a static array, growing the array
   as needed.  It is written to compile in both C11 and C++17. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache that stores already-computed a(k) for 0 <= k < cache_len. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid elements stored in cache */

    if ((size_t)n >= cache_len) {
        /* Grow the cache to contain up to index n. */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)          /* allocation failure – return 0 (undefined but safe) */
            return (seq_t)0;
        cache = tmp;

        /* Compute the new entries.  All required indices are already set. */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else
                cache[i] = cache[i / 2] + cache[i / 4] + cache[i / 8];
        }
        cache_len = new_len;
    }

    return cache[n];
}
