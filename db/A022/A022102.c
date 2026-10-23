/*
 * A022102  Fibonacci sequence beginning 1, 12.
 * https://oeis.org/A022102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 12
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array holding computed values */
static idx_t cache_size = 0;  /* number of stored terms (cache_size > 0 ⇒ array valid) */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)12;

    /* Return stored value if already computed */
    if (cache && n < cache_size)
        return cache[n];

    /* Initialise the cache the first time the function is called */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache)          /* allocation failure – let caller handle it */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
    }

    /* Extend the cache so that term a(n) exists */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;           /* required array length */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                                 /* allocation failure */
            return (seq_t)0;
        cache = tmp;

        /* Compute the new terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 2];

        cache_size = new_size;
    }

    return cache[n];
}
