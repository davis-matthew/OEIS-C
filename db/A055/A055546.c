/*
 * A055546  a(n) = (-1)^(n+1) * 2^n * n!^2.
 * https://oeis.org/A055546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1) * 2^n * (n!)^2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -1,  a(n+1) = -2 * (n+1)^2 * a(n)  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to handle arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* On first call, initialize cache[0] = -1 */
    if (cache_len == 0) {
        cache_cap = 4; /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure: return zero */
        cache[0] = -1;
        cache_len = 1;
    }

    /* Grow capacity if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute up to requested n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = prev;           /* start with a(i-1) */
        next *= -2;                  /* multiply by -2 */
        next *= (seq_t)i;             /* multiply by i */
        next *= (seq_t)i;             /* multiply by i again (i^2) */
        cache[i] = next;
    }
    cache_len = n + 1;

    return cache[n];
}
