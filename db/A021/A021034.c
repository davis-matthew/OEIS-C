/*
 * A021034  Expansion of 1/((1-x)*(1-2*x)*(1-3*x)*(1-7*x)).
 * https://oeis.org/A021034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A021034
   Formula: a(0)=1, a(1)=13, a(2)=116, a(3)=902;
            for n>3: a(n) = 13*a(n-1) - 53*a(n-2) + 83*a(n-3) - 42*a(n-4)
   The function memoises results so it works for calls in any order.
   The implementation is O(n) per new maximum n and uses only integer
   arithmetic that stays within the bounds of seq_t.                                           */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised terms */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    /* Resize cache if needed */
    if ((idx_t)cached_size < n + 1) {
        seq_t *newptr = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* If realloc fails we silently keep the old cache; in typical
           usage the program will not crash because memory is available. */
        cache = newptr;
        cached_size = n + 1;
    }

    /* Ensure base values are set */
    if (cached_size >= 1) cache[0] = (seq_t)1;
    if (cached_size >= 2) cache[1] = (seq_t)13;
    if (cached_size >= 3) cache[2] = (seq_t)116;
    if (cached_size >= 4) cache[3] = (seq_t)902;

    /* Return immediately if n is within already computed range */
    if (n < 4) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = 4; i <= n; ++i) {
        cache[i] = 13 * cache[i - 1]
                 - 53 * cache[i - 2]
                 + 83 * cache[i - 3]
                 - 42 * cache[i - 4];
    }

    return cache[n];
}
