/*
 * A042789  Denominators of continued fraction convergents to sqrt(925).
 * https://oeis.org/A042789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1764*a(n-5) + a(n-10) for n>9 initial values: a(0)=1,
 *            a(1)=2, a(2)=5, a(3)=12, a(4)=29, a(5)=1752, a(6)=3533,
 *            a(7)=8818, a(8)=21169, a(9)=51156
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity (>= n+1) */
    static size_t computed = 0; /* last index for which value is stored,
                                   0 means not initialized yet */

    /* make sure we have space for indices up to n */
    if ((size_t)n + 1 > cap)
        ensure_cache((idx_t)( (size_t)n + 1 ), &cache, &cap);

    /* Initialize base values once */
    if (computed == 0) {
        static const seq_t init[10] = {
            (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)12, (seq_t)29,
            (seq_t)1752, (seq_t)3533, (seq_t)8818, (seq_t)21169,
            (seq_t)51156
        };
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        /* zero any new slots that may have been allocated above */
        for (size_t i = 10; i < cap; ++i)
            cache[i] = 0;
        computed = 9;
    }

    /* Return if value already computed */
    if ((size_t)n <= computed)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 1764 * a(i-5) + a(i-10) */
        cache[i] = (seq_t)1764 * cache[i - 5] + cache[i - 10];
    }
    computed = (size_t)n;
    return cache[(size_t)n];
}
