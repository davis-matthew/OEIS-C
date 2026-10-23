/*
 * A025470  Expansion of 1/((1-2*x)*(1-3*x)*(1-4*x)*(1-9*x)).
 * https://oeis.org/A025470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n+1) - 2^(n+1) + 13*a(n-1) - 36*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* initial values: a(0)=1, a(1)=18 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static seq_t  pow2 = (seq_t)4;    /* 2^(last+1) */
    static seq_t  pow3 = (seq_t)9;    /* 3^(last+1) */
    static idx_t  last   = 1;         /* last index computed */

    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;

    /* first time: allocate cache for a(0) and a(1) */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* failure (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        pow2   = (seq_t)4;            /* 2^(1+1) */
        pow3   = (seq_t)9;            /* 3^(1+1) */
        last   = 1;
    }

    /* grow cache if needed */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* failure (unlikely) */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = pow3 * (seq_t)3;    /* 3^(i+1) */
        term -= pow2 * (seq_t)2;         /* 2^(i+1) */
        term += cache[i - 1] * (seq_t)13;
        term -= cache[i - 2] * (seq_t)36;
        cache[i] = term;

        /* update powers for next iteration */
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
    }

    last = n;
    return cache[n];
}
