/*
 * A160060  Numerator of Hermite(n, 14/25).
 * https://oeis.org/A160060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 14/25) Recurrence: a(0)=1, a(1)=28,
 *            a(n+1) = 28*a(n) - 1250*n*a(n-1) (for n >= 1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with a(0) */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;       /* a(0) */
    }

    /* extend cache up to required index n */
    while (cache_len <= n) {
        if (cache_len == 1) {
            /* compute a(1) directly */
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_len + 1));
            cache[cache_len] = (seq_t)28;    /* a(1) */
            cache_len++;                     /* now cache_len == 2 */
            continue;
        }

        /* compute a_k where k = cache_len (next index to fill) */
        idx_t k = cache_len;                /* index of the term to compute */
        seq_t prev_prev = cache[cache_len - 2];  /* a(k-2) */
        seq_t prev = cache[cache_len - 1];        /* a(k-1) */

        /* recurrence: a(k) = 28*a(k-1) - 1250*(k-1)*a(k-2) */
        seq_t term = (seq_t)28 * prev;
        seq_t coef = (seq_t)1250 * (cache_len - 1);  /* (k-1) */
        seq_t sub  = coef * prev_prev;
        seq_t next = term - sub;

        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_len + 1));
        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n];
}
