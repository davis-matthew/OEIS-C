/*
 * A211629  Number of ordered triples (w,x,y) with all terms in {-n, ..., -1,
 *          1, ..., n} and 4w + x + y > 0.
 * https://oeis.org/A211629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A211629
    a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 3*a(n-5) + 3*a(n-6) - a(n-7)
    Initial values:
        a(0)=0, a(1)=4, a(2)=31, a(3)=105, a(4)=252,
        a(5)=492, a(6)=851.
*/

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;          /* array of a(0)...a(k) */
    static size_t cache_cap = 0;         /* current allocation size */
    static idx_t last_computed = 0;      /* highest index computed so far */

    /* initialise cache with the first 7 terms (if not already done) */
    if (cache == NULL) {
        cache_cap = 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {            /* unlikely, but be defensive */
            return 0;
        }
        cache[0] = 0;    /* a(0) */
        cache[1] = 4;    /* a(1) */
        cache[2] = 31;   /* a(2) */
        cache[3] = 105;  /* a(3) */
        cache[4] = 252;  /* a(4) */
        cache[5] = 492;  /* a(5) */
        cache[6] = 851;  /* a(6) */
        last_computed = 6;
    }

    /* If requested term is already computed, return it */
    if (n <= (idx_t)last_computed) {
        return cache[(size_t)n];
    }

    /* Ensure capacity large enough for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {               /* allocation failed: return 0 */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* all computations in unsigned arithmetic, overflow is intentional
           for small seq_t widths. */
        seq_t t1 = cache[i - 1];
        t1 *= (seq_t)3;          /* 3*a(i-1) */
        t1 += cache[i - 3];      /* + a(i-3) */
        t1 += cache[i - 4];      /* + a(i-4) */
        t1 += cache[i - 6] * (seq_t)3; /* + 3*a(i-6) */

        seq_t t2 = cache[i - 2];
        t2 *= (seq_t)3;          /* 3*a(i-2) */
        t2 += cache[i - 5] * (seq_t)3; /* + 3*a(i-5) */
        t2 += cache[i - 7];      /* + a(i-7) */

        cache[i] = t1 - t2;      /* guaranteed non‑negative in exact arithmetic */
    }

    last_computed = n;
    return cache[(size_t)n];
}
