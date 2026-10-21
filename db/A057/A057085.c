/*
 * A057085  a(n) = 9*a(n-1) - 9*a(n-2) for n>1, with a(0)=0, a(1)=1.
 * https://oeis.org/A057085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1, a(n) = 9*a(n-1) - 9*a(n-2), n > 1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to handle arbitrary call order and improve speed */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently allocated */
    static idx_t last = 0;        /* index of the highest computed element */

    /* Ensure cache is initialized for at least the first two elements */
    if (cache == NULL) {
        cache_len = 4;                      /* start with some room */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;                      /* a(0) */
        cache[1] = 1;                      /* a(1) */
        last = 1;
    }

    /* If requested index already computed, return it directly */
    if (n <= last)
        return cache[n];

    /* Grow cache if needed */
    if (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        while (new_len <= n) new_len *= 2;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * 9;
        seq_t t2 = cache[i - 2] * 9;
        cache[i] = t1 - t2;
    }
    last = n;
    return cache[n];
}
