/*
 * A016166  Expansion of 1/((1-5*x)*(1-12*x)).
 * https://oeis.org/A016166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12^(n+1) - 5^(n+1))/7 Recurrence (used for fast
 *            computation): a(0) = 1 a(n) = 12*a(n-1) + 5^n for n ≥ 1 This
 *            function caches computed values so repeated calls are fast
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;      /* array of sequence values */
    static size_t cache_size = 0;    /* number of elements actually stored */
    static size_t cache_cap  = 0;    /* allocated capacity */
    static seq_t pow5 = 1;           /* current value of 5^i, where i = cache_size */

    /* initial allocation: a(0) = 1 */
    if (cache == NULL) {
        cache_cap   = 1;
        cache_size  = 1;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;  /* allocation failed – return zero */
        cache[0] = (seq_t)1;
        pow5     = (seq_t)1;  /* 5^0 */
    }

    /* extend cache up to the requested index n */
    while (cache_size <= n) {
        /* grow the allocated array if needed */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 1 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                return (seq_t)0;  /* handle allocation failure */
            cache       = tmp;
            cache_cap   = new_cap;
        }

        /* compute next term a(cache_size) */
        size_t i = cache_size;
        pow5 = pow5 * (seq_t)5;                          /* 5^i */
        cache[i] = cache[i-1] * (seq_t)12 + pow5;        /* a(i) = 12*a(i-1)+5^i */
        cache_size++;
    }

    return cache[n];
}
