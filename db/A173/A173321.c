/*
 * A173321  a(n) = 4*n! - 1.
 * https://oeis.org/A173321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A173321: a(n) = 4*n! - 1
 * Recurrence: a(0) = 3,  a(n) = n*a(n-1) + n - 1  (n > 0)
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache to handle arbitrary call order */
    static seq_t *cache       = NULL; /* array of computed a(i) */
    static size_t cache_size = 0;     /* allocated size */
    static idx_t   next       = 0;     /* next index to compute */

    /* trivial case */
    if (n == 0)
        return (seq_t)3;

    /* initialise cache if first call */
    if (cache == NULL) {
        /* allocate space for at least n+1 entries */
        cache_size = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* fallback on allocation failure */
        cache[0] = (seq_t)3;
        next = 1;
    }
    /* extend cache if it is not big enough */
    else if ((size_t)n >= cache_size) {
        size_t new_size = cache_size ? cache_size : 1;
        while ((size_t)n >= new_size)
            new_size <<= 1;               /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* fallback on reallocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = next; i <= n; ++i) {
        /* cast i to seq_t before multiplication to avoid overflow of
           intermediate idx_t */
        cache[i] = (seq_t)i * cache[i - 1] + (seq_t)i - (seq_t)1;
    }
    next = n + 1; /* next time we have up to n computed */

    return cache[n];
}
