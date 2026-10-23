/*
 * A016279  Expansion of 1/((1-2x)(1-3x)(1-10x)).
 * https://oeis.org/A016279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 56*a(n-2) + 60*a(n-3), initial values:
 *            a(0)=1, a(1)=15, a(2)=169
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Initialise cache with the first three terms */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)169;
    }

    /* If the requested term is already computed, return it. */
    if ((size_t)n < cap)
        return cache[n];

    /* Expand the cache to include the requested index. */
    size_t oldcap = cap;
    size_t newcap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    /* If allocation fails, the behaviour is undefined in this context,
       but for normal use the allocation should succeed. */
    cache = tmp;
    cap = newcap;

    /* Compute missing terms using the recurrence. */
    for (size_t i = oldcap; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)15 * cache[i-1]
                 - (seq_t)56 * cache[i-2]
                 + (seq_t)60 * cache[i-3];
    }

    return cache[n];
}
