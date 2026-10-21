/*
 * A061279  a(n) = Sum_{k >= 0} 2^k * binomial(k+2,n-2*k).
 * https://oeis.org/A061279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3), a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic cache for computed values    */
    static idx_t  cache_n = 0;    /* number of computed terms (size)      */
    static idx_t  cache_cap = 0;  /* current allocation size              */

    /* hard‑coded initial values for 0,1,2 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    /* first call – allocate and initialise cache */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 3);
        if (!cache) return 0;                /* out‑of‑memory fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache_n  = 3;
        cache_cap = 3;
    }

    /* if n is beyond cached values, grow and compute missing terms */
    if (n >= cache_n) {
        idx_t new_cap = n + 1;               /* after computation cache will contain indices 0..n   */
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0;              /* out‑of‑memory fallback */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_n; i <= n; ++i) {
            /* a(i) = 2*a(i-2) + 2*a(i-3) */
            seq_t t = cache[i - 2];
            t += t;                /* t = 2*a(i-2) */
            t += cache[i - 3];
            t += cache[i - 3];      /* t = 2*a(i-2) + 2*a(i-3) */
            cache[i] = t;
        }
        cache_n = n + 1;
    }

    return cache[n];
}
