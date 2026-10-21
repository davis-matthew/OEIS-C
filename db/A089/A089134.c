/*
 * A089134  a(n) = -a(n-1) - a(n-2) + a(n-3) - a(n-5).
 * https://oeis.org/A089134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) - a(n-2) + a(n-3) - a(n-5);
   Initial values: a(1) = -1, a(2) = 0, a(3) = 2, a(4) = -3, a(5) = 0 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), ... */
    static idx_t cache_size = 0;   /* number of terms stored */

    /* Initialize cache with the first five terms */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)(-1);   /* a(1) */
        cache[1] = (seq_t)0;      /* a(2) */
        cache[2] = (seq_t)2;      /* a(3) */
        cache[3] = (seq_t)(-3);   /* a(4) */
        cache[4] = (seq_t)0;      /* a(5) */
    }

    /* Return early if the requested term is already cached */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Extend cache up to the requested index */
    idx_t new_size = n;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
        /* recurrence: a(idx) = -a(idx-1) - a(idx-2) + a(idx-3) - a(idx-5) */
        seq_t term = -cache[idx - 2];     /* -a(idx-1) */
        term -= cache[idx - 3];           /* -a(idx-2) */
        term += cache[idx - 4];           /* +a(idx-3) */
        term -= cache[idx - 6];           /* -a(idx-5) */
        cache[idx - 1] = term;
    }

    cache_size = n;
    return cache[n - 1];
}
