/*
 * A157144  Hankel transform of A157143.
 * https://oeis.org/A157144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 30, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* 
 * Formula: a(n) = -3*a(n-3) - 8*a(n-6) - 3*a(n-9) - a(n-12)  (for n >= 12)
 * with initial terms:
 * a(0) =  1, a(1) = -2,  a(2) = -3,  a(3) = -3,  a(4) =  8,
 * a(5) =  0, a(6) =  0,  a(7) = -8,  a(8) = 21,  a(9) = 21,
 * a(10) = -34, a(11) = -55
 */

/* initial values of the sequence */
static const seq_t init_vals[12] = {
    1, -2, -3, -3, 8,
    0, 0, -8, 21, 21,
    -34, -55
};

/* storage for computed values; grows as needed */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* compute a(n) */
seq_t a(idx_t n)
{
    /* use the known initial segment for small n */
    if (n < 12)
        return init_vals[n];

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_len = 12;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
    }

    /* extend the cache up to the requested index */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();    /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a[i] = -a[i-12] - 3*a[i-9] - 8*a[i-6] - 3*a[i-3] */
            cache[i] = -cache[i-12]
                       - (seq_t)3 * cache[i-9]
                       - (seq_t)8 * cache[i-6]
                       - (seq_t)3 * cache[i-3];
        }
        cache_len = new_len;
    }

    return cache[n];
}
