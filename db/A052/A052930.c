/*
 * A052930  Expansion of (1-x)/(1 - x - 2*x^2 - 2*x^3 + 2*x^4).
 * https://oeis.org/A052930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) - 2*a(n-4) Initial values:
 *            a(0)=1, a(1)=0, a(2)=2, a(3)=4, a(4)=6 Recurrence is valid for
 *            n>=4
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* number of values stored   */
    static size_t cache_cap = 0;    /* allocated capacity       */

    /* Ensure initial values are available */
    if (cache_len == 0) {
        cache_cap = 8;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* set first five terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)6;
        cache_len = 5;
    }

    /* If requested value already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to at least n+1 entries */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;
    if (new_cap != cache_cap) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + 2*a(i-2) + 2*a(i-3) - 2*a(i-4) */
        seq_t val = cache[i - 1];
        val += cache[i - 2];
        val += cache[i - 2];
        val += cache[i - 3];
        val += cache[i - 3];
        val -= cache[i - 4];
        val -= cache[i - 4];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
