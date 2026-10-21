/*
 * A084157  a(n) = 8*a(n-1) - 16*a(n-2) + 12*a(n-4) with a(0)=0, a(1)=1,
 *          a(2)=4, a(3)=22.
 * https://oeis.org/A084157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 16*a(n-2) + 12*a(n-4), with a(0)=0, a(1)=1,
 *            a(2)=4, a(3)=22
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute term a(n) of OEIS A084157.
 * The function is memoizing results in a static array so that calls
 * arrive in any order and remain O(n) in the highest requested index.
 */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;          /* array holding values 0..cache_limit */
    static idx_t   cache_limit = 0;     /* highest index for which cache is valid */

    /* Initialise cache with base values */
    if (cache == NULL) {
        /* Allocate space for first four terms */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)22;
        cache_limit = 3;
    }

    /* Return directly if already computed */
    if (n <= cache_limit) {
        return cache[n];
    }

    /* Increase the cache to hold values up to n */
    idx_t new_limit = n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_limit + 1) * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failure – in a test harness this should never happen,
         * but we guard against it by returning zero. */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute new values using the recurrence.  The loop starts at
     * cache_limit + 1 to avoid recomputing already cached terms. */
    for (idx_t i = cache_limit + 1; i <= new_limit; ++i) {
        /* For i < 4 the recurrence would access negative indices;
         * but this loop never reaches i < 4 because cache_limit starts at 3. */
        seq_t term = (seq_t)8 * cache[i - 1]  /* 8*a(n-1) */
                  - (seq_t)16 * cache[i - 2] /* -16*a(n-2) */
                  + (seq_t)12 * cache[i - 4];/* +12*a(n-4) */
        cache[i] = term;
    }

    cache_limit = new_limit;
    return cache[n];
}
