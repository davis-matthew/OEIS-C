/*
 * A072196  Multiples of 3 which on one operation of the Collatz function T (N
 *          -> 3N+1/2^r) yield the number 5.
 * https://oeis.org/A072196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 65*a(n-1) - 64*a(n-2), a(1)=3, a(2)=213
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* We cache computed values so that multiple calls return
       efficiently, independent of call order. */
    static seq_t *cache   = NULL;      /* cached terms (a(1) -> cache[0]) */
    static idx_t  cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity in cache */

    /* Handle the offset: a(1) is first term. */
    if (n == 0)
        return (seq_t)0;                /* never used for offset 1 */

    /* Initialise cache with a(1) and a(2). */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)3;                /* a(1) */
        cache[1] = (seq_t)213;              /* a(2) */
        cache_len = 2;
    }

    /* Extend cache up to the required index. */
    size_t target = (size_t)n;        /* n <= 10000 as per problem */
    if (target > cache_len) {
        /* Grow capacity if needed */
        if (cache_cap < target) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < target) new_cap = target;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!cache) return (seq_t)0;  /* allocation failure */
            cache_cap = new_cap;
        }

        /* Compute new terms using the recurrence. */
        for (size_t i = cache_len; i < target; ++i) {
            /* i is 0-based; an is cache[i] */
            cache[i] = (seq_t)((seq_t)65 * cache[i-1] -
                               (seq_t)64 * cache[i-2]);
        }
        cache_len = target;
    }

    return cache[n-1];
}
