/*
 * A102206  a(0) = 3, a(1) = 8, a(n+2) = 4*a(n+1) - a(n) - 2.
 * https://oeis.org/A102206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3, a(1) = 8, a(n+2) = 4*a(n+1) - a(n) - 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size  = 0;
    static idx_t last  = 0;

    /* Initialise or grow cache */
    if (cache == NULL) {
        size = (n < 2 ? 2 : n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        if (cache == NULL) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)8;
        last = 1;
    }
    else if (n >= size) {
        size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * size);
        if (cache == NULL) return (seq_t)0;           /* allocation failure */
    }

    /* Compute new terms if requested */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = (seq_t)4 * cache[i - 1] - cache[i - 2] - (seq_t)2;
        }
        last = n;
    }

    return cache[n];
}
