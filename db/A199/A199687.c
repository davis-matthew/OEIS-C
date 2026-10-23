/*
 * A199687  a(n) = 7*10^n + 1.
 * https://oeis.org/A199687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*10^n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used for efficient computation: a(0) = 8, a(n) = 10*a(n-1)-9 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache with a(0) */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) abort();            /* unlikely but safe */
        cache[0] = (seq_t)8;                   /* a(0) = 8 */
        cache_size = 1;
    }

    /* if requested value is already in cache, return it */
    if (n < cache_size)
        return cache[n];

    /* extend cache to accommodate a(n) */
    size_t old_size = cache_size;
    if (cache_size <= n) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();             /* unlikely but safe */
        cache = tmp;
        cache_size = n + 1;
    }

    /* compute missing values using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        /* i > 0 because old_size >= 1 (a(0) is present) */
        cache[i] = cache[i - 1] * ((seq_t)10) - (seq_t)9;
    }

    return cache[n];
}
