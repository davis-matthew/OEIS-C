/*
 * A214826  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) = 1, a(1) = a(2) = 4.
 * https://oeis.org/A214826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = 1, a(1) = 4, a(2) = 4
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for all computed i < len. */
    static seq_t *cache = NULL;
    static size_t cap = 0;  /* allocated capacity */
    static size_t len = 0;  /* number of entries computed (len <= cap) */

    /* Ensure we have at least three initial values */
    if (len == 0) {
        cap = 8;  /* small initial capacity */
        if (cache == NULL) {
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (cache == NULL) return 0;  /* allocation failure -> zero */
        }
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 4;
        len = 3;
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;        /* allocation failure -> zero */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    if (len <= (size_t)n) {
        for (idx_t i = len; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
