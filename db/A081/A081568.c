/*
 * A081568  Third binomial transform of Fibonacci(n+1).
 * https://oeis.org/A081568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A081568 – Third binomial transform of Fibonacci(n+1)
 * Formula: a(0) = 1,  a(1) = 4,
 *          a(n) = 7*a(n-1) - 11*a(n-2)   for n >= 2
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache previous terms to support arbitrary access order. */
    static seq_t *cache = NULL;   /* array of computed values          */
    static size_t  used  = 0;     /* number of values stored (cache[0..used-1]) */
    static size_t  cap   = 0;     /* current allocation size           */

    /* Ensure the cache has space for at least two elements
       so that the base values can be stored. */
    if (cap < 2) {
        size_t new_cap = 2;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;           /* out of memory: return 0 (unlikely) */
        cache = new_cache;
        cap   = new_cap;
        used  = 0;
    }

    /* Store the base values if they haven't been set yet. */
    if (used < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        used = 2;
    }

    /* If the requested index is already cached, return it immediately. */
    if ((size_t)n < used)
        return cache[(size_t)n];

    /* Grow the cache if needed to reach the requested index. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;           /* out of memory: return 0 (unlikely) */
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute successive terms up to n. */
    for (size_t i = used; i <= (size_t)n; ++i) {
        /* a(i) = 7*a(i-1) - 11*a(i-2) */
        cache[i] = cache[i-1] * ((seq_t)7) - cache[i-2] * ((seq_t)11);
    }
    used = (size_t)n + 1;
    return cache[(size_t)n];
}
