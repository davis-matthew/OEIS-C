/*
 * A077850  Expansion of (1-x)^(-1)/(1 - 2*x - x^2 + x^3).
 * https://oeis.org/A077850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077850: expansion of (1-x)^(-1)/(1-2*x-x^2+x^3)
   Formula: a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + a(n-4) for n>=4
   Initial values: a(0)=1, a(1)=3, a(2)=8, a(3)=19 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t size = 0;

    /* Initialize cache with first four terms */
    if (size == 0) {
        capacity = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)19;
        size = 4;
    }

    /* If the requested index is already computed, just return it */
    if (n < size)
        return cache[n];

    /* Grow the cache array if necessary */
    if (n + 1 > capacity) {
        idx_t newcap = capacity;
        while (newcap <= n) newcap <<= 1;          /* double until large enough */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        capacity = newcap;
    }

    /* Compute terms up to n using the recurrence relation */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val = 3 * cache[i - 1] - cache[i - 2] - 2 * cache[i - 3] + cache[i - 4];
        cache[i] = val;
    }
    size = n + 1;

    return cache[n];
}
