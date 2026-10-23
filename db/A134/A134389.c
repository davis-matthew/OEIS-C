/*
 * A134389  A transform of floor((n+2)/2) with Hankel transform
 *          floor((n+2)/2)*(cos(Pi*n/2) + sin(Pi*n/2)).
 * https://oeis.org/A134389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(3)=11 (n-1)*a(n) = 2*n*a(n-1) -
 *            4*(4-n)*a(n-2) - 8*(n-3)*a(n-3) (n>=4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed terms          */
static idx_t  cache_cap = 0;         /* allocated capacity (in terms)   */
static idx_t  cache_computed = 0;    /* first index not yet stored      */

/* Make sure the cache can hold indices up to 'need'.                      */
static void grow_cache(idx_t need)
{
    if (need < cache_cap) return;           /* already enough           */

    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
    while (new_cap <= need) new_cap <<= 1;   /* double until large enough */

    seq_t *old = cache;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* allocation failed       */
    cache = tmp;

    /* For class types we must construct freshly allocated objects.       */
    if (old != cache) {
        idx_t i;
        for (i = cache_cap; i < new_cap; ++i) {
#ifdef __cplusplus
            new (&cache[i]) seq_t();      /* placement new for C++    */
#endif
        }
    }

    cache_cap = new_cap;
}

/* ************************************************************ *
 *  Compute a(n) for any non‑negative integer n.                *
 *  Uses memoisation so that calls in arbitrary order are cheap.* *
 * ************************************************************ */
seq_t a(idx_t n)
{
    /* Initialise first four terms on the very first call.                */
    if (cache_computed == 0) {
        grow_cache(3);                      /* need indices 0..3          */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)11;
        cache_computed = 4;                 /* indices 0..3 filled        */
    }

    /* If n is already computed, return it immediately.                  */
    if (n < cache_computed) return cache[n];

    /* Make room for terms up to n.                                        */
    grow_cache(n);

    /* Compute missing terms from cache_computed up to n.                  */
    for (idx_t i = cache_computed; i <= n; ++i)
    {
        if (i < 4) continue;                 /* base cases already set    */

        seq_t t1 = ((seq_t)i * (seq_t)2)            * cache[i - 1];
        seq_t t2 = ((seq_t)(i - 4) * (seq_t)4)      * cache[i - 2];
        seq_t t3 = ((seq_t)(i - 3) * (seq_t)8)      * cache[i - 3];

        seq_t numerator = t1 + t2 - t3;          /* (2i)*a(i-1)+(4i-16)*a(i-2)-8(i-3)*a(i-3) */
        cache[i] = numerator / ((seq_t)(i - 1));  /* divide by (i-1)                    */
    }

    cache_computed = n + 1;                    /* all indices up to n present     */
    return cache[n];
}
