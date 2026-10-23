/*
 * A192808  Constant term in the reduction of the polynomial (x^2 + 2)^n by
 *          x^3 -> x^2 + 2. See Comments.
 * https://oeis.org/A192808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n‑1) - 12*a(n‑2) + 8*a(n‑3), a(0) = 1, a(1) = 2, a(2)
 *            = 6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* Compute the nth term of OEIS A192808 */
seq_t a(idx_t n)
{
    /* Static cache to support out‑of‑order queries */
    static seq_t *cache     = NULL;   /* pointer to stored terms        */
    static idx_t  cache_sz = 0;       /* number of valid entries in cache*/
    static size_t cache_cap = 0;      /* allocated capacity in elements   */

    /* Ensure we have space for indices [0 .. n] */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                 /* grow to a power of two */

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)                    /* allocation failure */
            return (seq_t)0;               /* caller may ignore */

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate any missing terms up to n */
    for (idx_t i = cache_sz; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)2;
        else if (i == 2)
            cache[i] = (seq_t)6;
        else {
            seq_t t1 = cache[i-1] * (seq_t)7;
            seq_t t2 = cache[i-2] * (seq_t)12;
            seq_t t3 = cache[i-3] * (seq_t)8;
            cache[i] = t1 - t2 + t3;
        }
    }

    if (cache_sz <= n)
        cache_sz = n + 1;

    return cache[n];
}
