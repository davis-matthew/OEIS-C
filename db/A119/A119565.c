/*
 * A119565  a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=7, a(5)=10; a(n) =
 *          floor(a(n-1) + 1 + a(n-2)/6) for n>=6.
 * https://oeis.org/A119565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 67, u32 n <= 149, u64 n <= 312,
 *            u128 n <= 638, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A119565
   a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=7, a(5)=10;
   a(n) = floor( a(n-1) + 1 + a(n-2)/6 )   for n >= 6
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache     = NULL;   /* pointer to cache array      */
    static size_t cap       = 0;      /* allocated size              */
    static size_t computed  = 0;      /* number of initialized entries */

    /* initialisation: first six terms */
    if (!cache) {
        cap      = 6;
        cache    = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)10;

        computed = 6;
    }

    /* fast path for already computed indices */
    if ((size_t)n < computed)
        return cache[n];

    /* ensure space for indices up to n */
    size_t new_cap = cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                     /* grow to > n */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;             /* allocation failure */

    cache = tmp;
    cap   = new_cap;

    /* compute missing terms from the first uninitialised index up to n */
    idx_t start = (idx_t)computed;          /* first index to compute */
    for (idx_t i = start; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = prev1 + (seq_t)1 + (prev2 / (seq_t)6);
    }

    computed = (size_t)n + 1;               /* we now have terms 0..n */
    return cache[n];
}
