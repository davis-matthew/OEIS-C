/*
 * A025164  a(n) = a(n-2) + (2n-1)a(n-1); a(0)=1, a(1)=1.
 * https://oeis.org/A025164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + (2*n-1)a(n-1) for n ≥ 2, a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of values stored, indices 0..cache_len-1 */

    /* Initialize cache with a(0) and a(1) */
    if (cache == NULL) {
        cache_cap = 4;              /* start with small capacity */
        cache_len = 2;              /* indices 0 and 1 are stored */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* Extend the cache up to n */
    while (cache_len <= n) {
        idx_t i = cache_len; /* current index to compute */

        /* Compute a(i) using the recurrence */
        seq_t val = cache[i - 2] + ((seq_t)(2 * i - 1)) * cache[i - 1];

        /* Ensure we have enough capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0)
                new_cap = 4;
            if (new_cap < cache_len + 1)
                new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the newly computed term */
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n];
}
