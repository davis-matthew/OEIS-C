/*
 * A160068  Numerator of Hermite(n, 24/25).
 * https://oeis.org/A160068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 48
   a(n+2) = -1250*(n+1)*a(n) + 48*a(n+1)   (n >= 0) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* If this is the first call, allocate and initialise a(0), a(1) */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)48;
        cache_len = 2;
    }

    /* Expand cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = -1250*(i-1)*a(i-2) + 48*a(i-1)   */
        seq_t t1 = (seq_t)1250 * (seq_t)(i - 1);
        seq_t val1 = t1 * cache[i - 2];
        seq_t val2 = (seq_t)48 * cache[i - 1];
        cache[i] = -val1 + val2;
    }
    cache_len = n + 1;

    return cache[n];
}
