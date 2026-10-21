/*
 * A081911  a(n) = 5^n*(n^2 - n + 50)/50.
 * https://oeis.org/A081911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n*(n^2 - n + 50)/50 Recurrence: a(0)=1, a(1)=5,
 *            a(2)=26, a(n)=15*a(n-1) - 75*a(n-2) + 125*a(n-3) (n ≥ 3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* base values */
    static const seq_t a0 = (seq_t)1;
    static const seq_t a1 = (seq_t)5;
    static const seq_t a2 = (seq_t)26;

    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* direct return for the first three indices */
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    /* initialise cache to hold the first three values */
    if (!cache) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed – return 0 */
        cache[0] = a0;
        cache[1] = a1;
        cache[2] = a2;
    }

    /* grow cache up to index n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failed – return 0 */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t = cache[i-1] * (seq_t)15;           /* 15*a(n-1) */
            t = t - cache[i-2] * (seq_t)75;           /* -75*a(n-2) */
            t = t + cache[i-3] * (seq_t)125;          /* +125*a(n-3) */
            cache[i] = t;
        }
        cache_size = new_size;
    }

    return cache[n];
}
