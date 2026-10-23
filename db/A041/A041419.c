/*
 * A041419  Denominators of continued fraction convergents to sqrt(224).
 * https://oeis.org/A041419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 30*a(n-2) - a(n-4) for n > 3, a(0)=1, a(1)=1, a(2)=29,
 *            a(3)=30
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */

    /* Ensure the cache can hold element n */
    if (n >= ((idx_t)cache_size)) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        assert(tmp != NULL);          /* crash on allocation failure */
        cache = tmp;

        /* Initialize the new entries */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i == 2) {
                cache[2] = (seq_t)29;
            } else if (i == 3) {
                cache[3] = (seq_t)30;
            } else {
                /* recurrence: a(i) = 30*a(i-2) - a(i-4) */
                seq_t t = (seq_t)30 * cache[i - 2];
                cache[i] = t - cache[i - 4];
            }
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
