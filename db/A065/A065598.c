/*
 * A065598  a(0)=0, a(1)=1, a(2)=2; for n >= 3, a(n) = 2*a(n-1)*a(n-2) -
 *          a(n-3).
 * https://oeis.org/A065598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=0, a(1)=1, a(2)=2; for n>=3,
   a(n)=2*a(n-1)*a(n-2)-a(n-3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of cache */
    static idx_t  last  = 0;   /* index of last computed value */

    /* First time: initialise cache with 3 values */
    if (cache == NULL) {
        cap  = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        last = 2;
    }

    /* If the requested term is already cached, return it */
    if (n < cap) {
        return cache[n];
    }

    /* Grow cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t i = (size_t)(last + 1); i <= (size_t)n; ++i) {
        /* a(i) = 2*a(i-1)*a(i-2) - a(i-3) */
        cache[i] = (seq_t)2 * cache[i - 1] * cache[i - 2] - cache[i - 3];
    }
    last = n;

    return cache[n];
}
