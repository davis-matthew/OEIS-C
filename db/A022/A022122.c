/*
 * A022122  Fibonacci sequence beginning 3, 10.
 * https://oeis.org/A022122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0)=3, a(1)=10
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the oeis sequence A022122: Fibonacci sequence beginning 3, 10 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t   cached = 0;
    static idx_t   cap    = 0;

    /* Ensure the cache contains at least the first two terms */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {
            /* In an embedded environment a failed allocation is fatal */
            return (seq_t)0;
        }
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)10;
        cached = 2;
    }

    /* Extend the cache if necessary */
    while (n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) {
            return (seq_t)0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* If the requested value is already computed, return it */
    if (n < cached) {
        return cache[n];
    }

    /* Compute missing terms iteratively */
    for (idx_t i = cached; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2];
    }
    cached = n + 1;
    return cache[n];
}
