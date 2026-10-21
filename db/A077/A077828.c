/*
 * A077828  Expansion of 1/(1-3*x-3*x^2-3*x^3).
 * https://oeis.org/A077828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 3*a(n-3), a(0)=1, a(1)=3, a(2)=12
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow memoised calls in arbitrary order */
    static seq_t *cache = NULL;
    static idx_t  last  = (idx_t)(-1);   /* indicates uninitialised */
    static size_t cap   = 0;

    /* Initialise cache with first three terms if needed */
    if (last == (idx_t)-1) {
        cap = 8;                           /* starting capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* out of memory; return 0 as safe default */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
        last = 2;
    }

    /* Check already computed */
    if (n <= last) {
        return cache[n];
    }

    /* Ensure enough space */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i < 3) {                      /* safety, should not happen */
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)3;
            else if (i == 2) cache[i] = (seq_t)12;
            continue;
        }
        seq_t t1 = cache[i-1] * (seq_t)3;
        seq_t t2 = cache[i-2] * (seq_t)3;
        seq_t t3 = cache[i-3] * (seq_t)3;
        cache[i] = t1 + t2 + t3;
    }
    last = n;
    return cache[n];
}
