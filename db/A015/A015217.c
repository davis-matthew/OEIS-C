/*
 * A015217  Sum of Gaussian binomial coefficients for q=24.
 * https://oeis.org/A015217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2, a(n) = 2*a(n-1) + a(n-2)*((24^(n-1)) - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;     /* array of terms */
    static size_t cap = 0;          /* allocated capacity */
    static idx_t last = 0;          /* highest index cached */
    static seq_t pow24 = 0;         /* 24^(current_index-1) */
    static idx_t pow_exp = 0;       /* exponent of pow24: pow24 = 24^pow_exp */
    static int init = 0;

    if (!init) {
        /* initialise cache for indices 0 and 1 */
        size_t init_cap = (n > 1) ? ((size_t)n + 1) : 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
        if (!cache) return 0; /* out of memory, return 0 as safe fallback */
        cache[0] = (seq_t)1;
        if (init_cap > 1) cache[1] = (seq_t)2;
        cap = init_cap;
        last = 1;
        /* pow24 stores 24^(1) for computing a(2) */
        pow24 = (seq_t)24;
        pow_exp = 1;
        init = 1;
    }

    if (n <= last) return cache[n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0; /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = 0;
        /* term = 2*cache[i-1] + cache[i-2]*(pow24-1) */
        term = cache[i-1];
        term += cache[i-1];          /* 2*cache[i-1] */
        seq_t mul = cache[i-2] * (pow24 - (seq_t)1);
        term += mul;
        cache[i] = term;
        /* update pow24 for next iteration: multiply by 24 */
        pow24 = pow24 * (seq_t)24;
        ++pow_exp;
    }

    last = n;
    return cache[n];
}
