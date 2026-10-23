/*
 * A162937  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A162937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) + 4*a(n-3) - 10*a(n-4) for n >= 5
 *            with initial values a(0)=1, a(1)=6, a(2)=30, a(3)=150, a(4)=735.
 *            This follows from the generating function
 *            (t^4+2t^3+2t^2+2t+1)/(10t^4-4t^3-4t^2-4t+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;     /* array of a[0..len-1] */
    static size_t cap  = 0;         /* allocated capacity */
    static idx_t   len  = 0;        /* number of terms currently stored */

    /* initial terms for n=0..4 */
    static const seq_t init[5] = { (seq_t)1, (seq_t)6, (seq_t)30,
                                   (seq_t)150, (seq_t)735 };

    /* if n is small, we can return the hard‑coded value directly
       (but we still need the cache for future larger n) */
    if (n < 5) {
        /* ensure cache is allocated so that larger n can be handled later */
        if (!cache) {
            cap = (n >= 4 ? n + 1 : 5) + 5;  /* some spare space */
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            len    = 5;
            for (size_t i = 0; i < 5; ++i)
                cache[i] = init[i];
        }
        return cache[(size_t)n];
    }

    /* n >= 5 */
    if (!cache) {
        /* first time we need more than 4 terms */
        cap = n + 1 + 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        len = 5;
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
    }

    /* grow capacity if necessary */
    if ((size_t)cap < (size_t)n + 1) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* out of memory – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t value = (seq_t)4 * cache[i - 1]
                    + (seq_t)4 * cache[i - 2]
                    + (seq_t)4 * cache[i - 3];
        value -= (seq_t)10 * cache[i - 4];
        cache[i] = value;
    }

    len = n + 1;
    return cache[(size_t)n];
}
