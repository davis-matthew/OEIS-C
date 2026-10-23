/*
 * A145696  Numbers Y such that 111*Y^2+37 is a square.
 * https://oeis.org/A145696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 590*a(n+1) - a(n) Initial values: a(1) = 7, a(2) = 4137
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Reject indices below the offset – behaviour is undefined. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;  /* pointer to cached terms          */
    static size_t cache_len = 0; /* number of initialised elements   */

    /* Initialise cache with the first two terms, if required. */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)7;      /* a(1) */
        cache[1] = (seq_t)4137;   /* a(2) */
        cache_len = 2;
    }

    /* Return a cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend the cache to contain a(n). */
    size_t new_len = (size_t)n;
    cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!cache) return (seq_t)0;

    /* Recurrence: a(k) = 590*a(k-1) - a(k-2) */
    seq_t prev2 = cache[cache_len - 2]; /* a(k-2)  */
    seq_t prev1 = cache[cache_len - 1]; /* a(k-1)  */

    for (size_t i = cache_len + 1; i <= new_len; ++i) {
        seq_t next = (seq_t)590 * prev1 - prev2;
        cache[i - 1] = next;
        prev2 = prev1;
        prev1 = next;
    }

    cache_len = new_len;
    return cache[n - 1];
}
