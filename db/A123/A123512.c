/*
 * A123512  Arises in the normal ordering of functions of a*(a+)*a, where a
 *          and a+ are the boson annihilation and creation operators,
 *          respectively.
 * https://oeis.org/A123512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A123512 * n*a(n) = 2*n*(n+4)*a(n-1) -
 *            (n-1)*(n+3)*(n+4)*a(n-2) * Initial values: a(0)=1, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* array of computed terms */
    static idx_t  cache_cap = 0;    /* total allocated slots   */
    static idx_t  cache_max = 0;    /* highest index computed */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 2;                       /* slots for a(0) and a(1) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                 /* fatal if allocation fails */
        cache[0] = (seq_t)1;                 /* a(0) */
        cache[1] = (seq_t)10;                /* a(1) */
        cache_max = 1;
    }

    /* enlarge the cache if required */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        /* ensure capacity > n so we can store index n */
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* use seq_t for all intermediate arithmetic – handles
               arbitrary‑precision builds, and works as a 128‑bit
               integer in C builds. */
            seq_t nn   = (seq_t)i;
            seq_t a1   = cache[i - 1];
            seq_t a2   = cache[i - 2];

            seq_t term1 = 2 * nn * (nn + (seq_t)4) * a1;
            seq_t term2 = (nn - (seq_t)1) * (nn + (seq_t)3)
                          * (nn + (seq_t)4) * a2;

            seq_t num   = term1 - term2;
            cache[i]    = num / nn;          /* division is exact */
        }
        cache_max = n;
    }

    return cache[n];
}
