/*
 * A223940  Sums of antidiagonals of A223968.
 * https://oeis.org/A223940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A223940.
   Recurrence:  a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 3*a(n-4) + a(n-5)
   Initial values:  a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=31. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For the first six terms return the constants directly. */
    if (n < 6) {
        static const seq_t init[6] = {1, 2, 4, 8, 16, 31};
        return init[n];
    }

    /* Static cache for memoisation of computed terms. */
    static seq_t *cache = NULL;        /* dynamically allocated array */
    static idx_t cache_size = 0;       /* total capacity of 'cache' */
    static idx_t max_computed = 5;     /* highest index that has been filled */

    /* Initialise the cache on first use. */
    if (!cache) {
        cache_size = 8; /* start with a small capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        static const seq_t init[6] = {1, 2, 4, 8, 16, 31};
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
    }

    /* Grow the cache if it cannot hold the requested index. */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) {
            new_size <<= 1;          /* double until we have enough space */
        }
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        cache_size = new_size;
    }

    /* Compute missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = cache[i-1];
        term += cache[i-2] * ((seq_t)4);
        term -= cache[i-3] * ((seq_t)3);
        term -= cache[i-4] * ((seq_t)3);
        term += cache[i-5];
        cache[i] = term;
    }
    if (n > max_computed)
        max_computed = n;

    return cache[n];
}
