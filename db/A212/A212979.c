/*
 * A212979  Number of (w,x,y) with all terms in {0,...,n} and range=average.
 * https://oeis.org/A212979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4) + a(n-5) * -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + a(n-9) * Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=7, a(4)=10, * a(5)=13, a(6)=19,
 *            a(7)=25, a(8)=34
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Base values for 0 <= n <= 8 */
    const seq_t base[9] = { (seq_t)1, (seq_t)1, (seq_t)1,
                            (seq_t)7, (seq_t)10, (seq_t)13,
                            (seq_t)19, (seq_t)25, (seq_t)34 };

    /* Initialise cache with the first 9 terms once */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)realloc(NULL, cache_len * sizeof(seq_t));
        if (!cache)
            return 0;  /* Allocation failure; unlikely in normal use */
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = base[i];
    }

    /* If the requested term is already in the cache, return it */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache up to the required index */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        return 0;  /* Allocation failure */
    cache = tmp;

    /* Fill in terms from cache_len up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* All operations use seq_t only, so the function works
         * for built‑in integer types as well as arbitrary‑precision types. */
        seq_t val =
              (seq_t)2 * cache[i-1]
            - (seq_t)2 * cache[i-2]
            + (seq_t)2 * cache[i-3]
            -        cache[i-4]
            +        cache[i-5]
            - (seq_t)2 * cache[i-6]
            + (seq_t)2 * cache[i-7]
            - (seq_t)2 * cache[i-8]
            +        cache[i-9];

        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
