/*
 * A033962  Trajectory of 1 under map n->9n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   For n > 0:
      a(n) = (a(n-1) is even) ? a(n-1)/2 : 9*a(n-1)+1
   This implements the trajectory of 1 under the map
   n → 9 n + 1 if n odd, n → n/2 if n even. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *cache = NULL;      /* cached values of a(k) 0 ≤ k < cache_len */
    static idx_t cache_len = 0;     /* number of cached elements */
    static idx_t cache_cap = 0;     /* capacity of allocated cache */

    /* initialise cache with a(0)=1 */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* extend cache up to n */
    while (cache_len <= n) {
        seq_t prev = cache[cache_len - 1];
        seq_t next;

        /* if prev is even: divide by 2; else multiply by 9 and add 1 */
        if ((prev & (seq_t)1) == (seq_t)0)
            next = prev / (seq_t)2;
        else
            next = prev * (seq_t)9 + (seq_t)1;

        /* grow cache if necessary (doubling policy) */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap << 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n];
}
