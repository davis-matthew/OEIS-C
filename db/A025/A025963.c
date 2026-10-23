/*
 * A025963  Expansion of 1/((1-2*x)*(1-4*x)*(1-5*x)*(1-11*x)).
 * https://oeis.org/A025963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(n+1)-2^(n+1))/2 + 16*a(n-1) - 55*a(n-2) * a(0) = 1,
 *            a(1) = 22
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cached a(0)…a(last) */
    static size_t cap = 0;              /* allocated size of cache   */
    static idx_t last = (idx_t)-1;      /* largest index computed    */
    static seq_t pow2 = 0;              /* 2^(last+1)  */
    static seq_t pow4 = 0;              /* 4^(last+1)  */

    /* lazy initialization of the first two terms */
    if (last == (idx_t)-1) {
        size_t init_cap = 2;             /* we need at least a(0), a(1) */
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;            /* a(0) */
        cache[1] = (seq_t)22;           /* a(1) */
        last = 1;
        cap = init_cap;
        /* powers for the next index (>=2) */
        pow2 = (seq_t)4;                /* 2^(last+1) = 2^2 */
        pow4 = (seq_t)16;               /* 4^(last+1) = 4^2 */
    }

    /* if n is already computed, return it */
    if (n <= last) {
        return cache[n];
    }

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed, return 0 as sentinel */
        cache = tmp;
        cap = newcap;
    }

    /* extend cache from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t nextPow2 = pow2 * (seq_t)2;   /* 2^(i+1) */
        seq_t nextPow4 = pow4 * (seq_t)4;   /* 4^(i+1) */

        /* (4^(i+1)-2^(i+1))/2  */
        seq_t term = (nextPow4 - nextPow2) / (seq_t)2;

        /* 16*a(i-1) - 55*a(i-2)   */
        term += cache[i - 1] * (seq_t)16;
        term -= cache[i - 2] * (seq_t)55;

        cache[i] = term;

        /* update powers for the next iteration */
        pow2 = nextPow2;
        pow4 = nextPow4;
    }

    last = n;
    return cache[n];
}
