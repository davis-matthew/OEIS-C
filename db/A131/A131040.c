/*
 * A131040  a(n) = (1/2+(1/2)*i*sqrt(11))^(n+1) +
 *          (1/2-(1/2)*i*sqrt(11))^(n+1), where i=sqrt(-1).
 * https://oeis.org/A131040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 3*a(n-2), a(0)=1, a(1)=-5
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131040 (Lucas sequence V(1,3)) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size = 0;   /* number of elements allocated */
    static idx_t filled = 0; /* highest index stored + 1 */

    /* initialise cache with first two values on first call */
    if (size == 0) {
        size = 16; /* start with a small capacity */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory, graceful fail */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)(-5);
        filled = 2;
    }

    /* If requested index already computed, return immediately */
    if (n < filled)
        return cache[n];

    /* Expand cache if necessary */
    if (n + 1 > size) {
        idx_t new_size = size;
        while (new_size < n + 1)
            new_size <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory */
        cache = tmp;
        size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = filled; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)(-5);
        } else {
            /* a(i) = a(i-1) - 3*a(i-2) */
            cache[i] = cache[i - 1] - (seq_t)3 * cache[i - 2];
        }
    }
    filled = n + 1;
    return cache[n];
}
