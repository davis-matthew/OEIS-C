/*
 * A020494  Expansion of 1/((1-5x)(1-9x)(1-10x)).
 * https://oeis.org/A020494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n/4 - 81*9^n/4 + 20*10^n Recurrence: a(0)=1, a(1)=24,
 *            a(2)=391 a(n)=24*a(n-1)-185*a(n-2)+450*a(n-3) for n>2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure space */
    if (!cache || cache_size <= n) {
        size_t needed = (size_t)(n + 1);
        if (!cache) {
            cache = (seq_t*)malloc(sizeof(seq_t) * needed);
            if (!cache) return 0;          /* out of memory */
        } else {
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * needed);
            if (!cache) return 0;          /* out of memory */
        }
        cache_size = needed;

        /* Initialize first three terms */
        cache[0] = (seq_t)1;
        if (cache_size > 1) cache[1] = (seq_t)24;
        if (cache_size > 2) cache[2] = (seq_t)391;
    }

    /* Return early if we already know the term */
    if (n <= 2) return cache[n];

    /* Compute remaining terms up to n */
    for (idx_t i = 3; i <= n; ++i) {
        cache[i] = (seq_t)24 * cache[i-1]
                 - (seq_t)185 * cache[i-2]
                 + (seq_t)450 * cache[i-3];
    }

    return cache[n];
}
