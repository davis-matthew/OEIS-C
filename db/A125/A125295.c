/*
 * A125295  Number of different non-self-crossing ways of moving a tower of
 *          Hanoi from one peg onto another peg.
 * https://oeis.org/A125295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n+1) = a(n)^2 * (a(n) + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises previously computed terms.
   It is valid in both C11 and C++17.  The cast of the result of
   malloc/realloc is required for the arbitrary‑precision build
   where seq_t is a class type. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with a(0) = 1 */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) {                 /* allocation failure – unlikely */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* If the value is already cached, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache to hold a(n) */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {                     /* allocation failure */
        return (seq_t)0;
    }
    cache = tmp;
    cache_size = new_size;

    /* Compute missing terms iteratively */
    for (idx_t i = cache_size - 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        /* a(i) = prev^2 * (prev + 1) */
        seq_t next = prev * prev * (prev + (seq_t)1);
        cache[i] = next;
    }

    return cache[n];
}
