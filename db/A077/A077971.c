/*
 * A077971  Expansion of 1/(1+x-x^2-2*x^3).
 * https://oeis.org/A077971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 41, i32 n <= 86, i64 n <= 173,
 *            i128 n <= 347, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) + a(n-2) + 2*a(n-3),
   with a(0)=1, a(1)=-1, a(2)=2. */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cached values */
    static idx_t size = 0;        /* number of cached terms */

    /* First call: initialise the first three terms */
    if (cache == NULL) {
        size = 3;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = -1;
        cache[2] = 2;
    }

    /* If needed, extend the cache up to n */
    if (n >= size) {
        idx_t newsize = n + 1;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        for (idx_t i = size; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            cache[i] = -a1 + a2 + 2 * a3;
        }
        size = newsize;
    }

    return cache[n];
}
