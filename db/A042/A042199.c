/*
 * A042199  Denominators of continued fraction convergents to sqrt(624).
 * https://oeis.org/A042199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=49, a(3)=50, a(n)=50*a(n-2)-a(n-4) for n>=4 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise first four terms when needed */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)49;
        cache[3] = (seq_t)50;
    }

    /* grow cache to store terms up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-2] * (seq_t)50 - cache[i-4];
        }
        cache_len = new_len;
    }

    return cache[n];
}
