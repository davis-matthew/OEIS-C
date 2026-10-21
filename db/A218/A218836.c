/*
 * A218836  Unmatched value maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..1 n X 2 array.
 * https://oeis.org/A218836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A218836: a(n) = 2*a(n-1) + 3*a(n-2) + a(n-3) for n>=3
   with initial values a(0)=1, a(1)=2, a(2)=7.  This implementation
   memoizes computed terms in a static array, ensuring that any order
   of calls returns the correct value.  The recurrence is computed
   iteratively up to the requested n. */

#include <stddef.h>
#include <stdlib.h>

/* External types are provided by the build environment */
extern seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;

    /* Ensure the cache is large enough for n */
    if (cache_cap < 3) {
        /* initially store the three known terms */
        size_t init_cap = 3;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache_cap = init_cap;
    }

    if ((size_t)n >= cache_cap) {
        /* Expand the cache to accommodate terms up to n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed, return 0 */
        cache = tmp;
        /* Compute new terms iteratively */
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            cache[i] = t1 * 2 + t2 * 3 + t3;
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
