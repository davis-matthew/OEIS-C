/*
 * A099450  Expansion of 1/(1 - 5*x + 7*x^2).
 * https://oeis.org/A099450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 21, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5, a(n)=5*a(n-1)-7*a(n-2) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of valid entries in cache */

    /* Ensure cache has at least two entries for base cases */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) {
            /* Out of memory – return 0 as a fallback */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache_size = 2;
    }

    /* Expand cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – return last known value as a fallback */
            return cache[cache_size - 1];
        }
        cache = tmp;
        if (n == 0) {
            /* Only need to compute a(0) */
            cache[0] = (seq_t)1;
            cache_size = 1;
        } else if (n == 1) {
            /* Cache already has two entries */
            cache_size = 2;
        } else {
            /* Compute missing entries from cache_size up to n */
            idx_t i;
            for (i = cache_size; i <= n; ++i) {
                if (i == 0) {
                    cache[i] = (seq_t)1;
                } else if (i == 1) {
                    cache[i] = (seq_t)5;
                } else {
                    cache[i] = (seq_t)(5 * cache[i-1] - 7 * cache[i-2]);
                }
            }
            cache_size = new_size;
        }
    }

    return cache[n];
}
