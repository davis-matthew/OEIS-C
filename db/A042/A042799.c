/*
 * A042799  Denominators of continued fraction convergents to sqrt(930).
 * https://oeis.org/A042799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 122*a(n-2) - a(n-4)
   Initial values: a(0) = 1, a(1) = 2, a(2) = 121, a(3) = 244 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached values */
    static idx_t cache_size = 0;         /* number of cached entries */

    /* we need indices 0 … n, so at least n+1 entries */
    idx_t required = n + 1;

    /* grow the cache if necessary */
    if (required > cache_size) {
        size_t new_bytes = (size_t)required * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, new_bytes);
        if (tmp == NULL) exit(1);        /* allocation failure */
        cache = tmp;

        /* compute any new entries */
        for (idx_t i = cache_size; i < required; ++i) {
            if (i == 0)          cache[i] = 1;
            else if (i == 1)     cache[i] = 2;
            else if (i == 2)     cache[i] = 121;
            else if (i == 3)     cache[i] = 244;
            else                cache[i] = (seq_t)122 * cache[i - 2] - cache[i - 4];
        }
        cache_size = required;
    }

    return cache[n];
}
