/*
 * A028205  Expansion of 1/((1-6x)(1-7x)(1-9x)(1-11x)).
 * https://oeis.org/A028205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) - 99*a(n-2) + 7^(n+1) - 6^(n+1), n >= 2 Initial
 *            values: a(0) = 1, a(1) = 33
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)33;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* Number of elements stored. */
    static size_t cache_cap = 0;   /* Current capacity of the array. */
    static seq_t pow6_last = 0;    /* 6^(next_index+1) for the next computation. */
    static seq_t pow7_last = 0;    /* 7^(next_index+1). */

    /* Initialise cache on the first call. */
    if (cache == NULL)
    {
        /* Allocate space for at least the first two terms. */
        cache_cap = 2;
        if (n + 1 > cache_cap)
            cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* Out‑of‑memory: return 0 as a safe fallback. */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)33;
        cache_len = 2;

        /* Prepare powers for the next index (which will be 2). */
        pow6_last = (seq_t)6;
        pow6_last *= (seq_t)6;   /* 6^2 */
        pow6_last *= (seq_t)6;   /* 6^3 */
        pow7_last = (seq_t)7;
        pow7_last *= (seq_t)7;   /* 7^2 */
        pow7_last *= (seq_t)7;   /* 7^3 */
    }
    else if (n + 1 > cache_cap)
    {
        /* Grow the cache if necessary. */
        size_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;  /* Out‑of‑memory. */
        cache_cap = new_cap;
    }

    /* Extend the cache up to the required index n. */
    while (cache_len <= n)
    {
        seq_t a_n_minus1 = cache[cache_len - 1];
        seq_t a_n_minus2 = cache[cache_len - 2];

        /* Compute the next term using the recurrence. */
        seq_t val = ( (seq_t)20 * a_n_minus1 )
                  - ( (seq_t)99 * a_n_minus2 )
                  + pow7_last
                  - pow6_last;

        cache[cache_len] = val;

        /* Update powers for the following index. */
        pow6_last *= (seq_t)6;
        pow7_last *= (seq_t)7;

        cache_len++;
    }

    return cache[n];
}
