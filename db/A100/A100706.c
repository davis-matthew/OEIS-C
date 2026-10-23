/*
 * A100706  Bisection of A002275.
 * https://oeis.org/A100706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-1) + 11, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently a(n) = (10^(2*n+1)-1)/9) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* number of entries stored */
    static idx_t cache_cap   = 0;   /* allocated capacity */

    /* initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cache_cap = 8;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;/* OOM – caller can decide */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* extend cache up to n, if needed */
    while (cache_len <= n) {
        /* compute next term using the recurrence */
        seq_t next = cache[cache_len - 1] * (seq_t)100 + (seq_t)11;

        /* grow array if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;   /* double capacity */
            seq_t *tmp =
                (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* OOM */
            cache = tmp;
        }
        cache[cache_len++] = next;
    }

    return cache[n];
}
