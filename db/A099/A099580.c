/*
 * A099580  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k, k-1) * 4^(k-1).
 * https://oeis.org/A099580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-2) - 4*a(n-3) - 16*a(n-4) Initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache      = NULL;
    static idx_t cache_cap  = 0;  /* allocated capacity */
    static idx_t cache_len  = 0;  /* number of elements stored */

    /* Make sure cache has initial values for indices 0..3. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache_len = 4;
    }

    /* Grow cache if needed. */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 4) {              /* already known */
            val = cache[i];
        } else {
            val = cache[i-1]
                + (seq_t)8  * cache[i-2]
                - (seq_t)4  * cache[i-3]
                - (seq_t)16 * cache[i-4];
        }
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
