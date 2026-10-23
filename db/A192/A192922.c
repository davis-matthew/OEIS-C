/*
 * A192922  Constant term in the reduction by (x^2->x+1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3) - a(n-4); initial values:
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 58, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t  cache_cap = 0;      /* allocated capacity of cache */
    static idx_t   cache_max = 0;      /* highest index already computed */

    /* handle first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* initialise cache if needed */
    if (!cache) {
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache_max = 3;
    }

    /* return cached value if already computed */
    if (n <= cache_max)
        return cache[n];

    /* grow cache to hold up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* compute all terms up to n, storing them */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        /* recurrence: a(i) = 2*a_{i-1} + 2*a_{i-2} - 3*a_{i-3} - a_{i-4} */
        seq_t res = (2 * a1 + 2 * a2) - (3 * a3 + a4);
        cache[i] = res;
    }

    cache_max = n;
    return cache[n];
}
