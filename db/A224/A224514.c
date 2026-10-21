/*
 * A224514  Expansion of
 *          (1-x)*(1-3*x)*(1-3*x+x^2)/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A224514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5);
 *            a(0)=1, a(1)=2, a(2)=6, a(3)=20, a(4)=70
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated capacity in elements   */
    static idx_t  best  = 0;          /* highest index that has been computed */

    /* initialisation on first call */
    if (cache == NULL) {
        cap = 5;                      /* store a(0)..a(4) initially      */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure – fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)70;
        best   = 4;
    }

    /* return immediately if already computed */
    if (n <= best) {
        return cache[(size_t)n];
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 1;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* if realloc fails, keep old cache; caller may be ill‑formed */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms in forward order */
    for (idx_t i = best + 1; i <= n; ++i) {
        seq_t v =  (cache[(size_t)(i - 1)] * (seq_t)9)
                  - (cache[(size_t)(i - 2)] * (seq_t)28)
                  + (cache[(size_t)(i - 3)] * (seq_t)35)
                  - (cache[(size_t)(i - 4)] * (seq_t)15)
                  +  cache[(size_t)(i - 5)];
        cache[(size_t)i] = v;
    }
    best = n;

    return cache[(size_t)n];
}
