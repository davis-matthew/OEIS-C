/*
 * A130449  a(0) = 1; a(n) = 4^(n+1)*a(n-1)+1.
 * https://oeis.org/A130449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 4^(n+1) * a(n-1) + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;    /* array of a(0) ... a(max_computed) */
    static idx_t   max_computed = 0; /* largest n for which cache is filled */
    static seq_t   power = 4;      /* holds 4^(max_computed+1) after computation */

    /* one‑time initialization */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;        /* allocation fail – unlikely in tests */
        cache[0] = (seq_t)1;                       /* a(0) = 1 */
        max_computed = 0;
        power = (seq_t)4;                          /* 4^(0+1) for next step */
    }

    /* if the requested value is already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* enlarge cache to hold up to n */
    idx_t old_max = max_computed;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)(n + 1));
    if (cache == NULL) return (seq_t)0;            /* allocation fail */

    /* continue the recurrence for indices old_max+1 .. n */
    for (idx_t i = old_max + 1; i <= n; ++i) {
        power *= (seq_t)4;                         /* 4^(i+1) from 4^i */
        cache[i] = power * cache[i - 1] + (seq_t)1;
    }

    max_computed = n;
    return cache[n];
}
