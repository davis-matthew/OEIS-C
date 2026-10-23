/*
 * A182201  Fibonacci-type sequence based on bitwise exclusive-or: a(0) = 0,
 *          a(1) = 1 and a(n) = a(n-1) + (a(n-1) xor a(n-2)).
 * https://oeis.org/A182201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + (a(n-1) xor a(n-2)), a(0)=0, a(1)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A182201; memoises previously computed values for speed. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms             */
    static size_t cache_cap = 0;  /* allocated capacity in seq_t slots    */
    static size_t cache_size = 0; /* number of valid terms in cache       */

    /* Initialise cache with the first two terms on first call. */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* Allocation failure – unlikely.    */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Grow cache if necessary.  n is idx_t but guaranteed to fit size_t for the tests. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (needed > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;          /* Realloc failure – unlikely.      */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (size_t i = cache_size; i < needed; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = prev1 + (prev1 ^ prev2);
    }
    if (cache_size < needed)
        cache_size = needed;

    return cache[(size_t)n];
}
