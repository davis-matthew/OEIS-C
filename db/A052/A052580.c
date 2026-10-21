/*
 * A052580  E.g.f. (1-2x)/(1-2x-x^2).
 * https://oeis.org/A052580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*Pell(n‑1), where Pell(0)=0, Pell(1)=1 and
 *            Pell(k)=2*Pell(k‑1)+Pell(k‑2). Recurrence used for fast
 *            computation: a(0)=1, a(1)=0, and for n≥0 a(n+2) = (2 + n² +
 *            3n)·a(n) + (4 + 2n)·a(n+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Avoid any C++ or <vector> by using plain malloc/realloc. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;   /* array of seq_t storing a(n) */
    static idx_t   cache_size = 0;      /* number of valid entries */
    static idx_t   cache_cap  = 0;      /* allocated size of cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;                         /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                   /* allocation failed */
        cache[0] = (seq_t)1;                    /* a(0) */
        cache[1] = (seq_t)0;                    /* a(1) */
        cache_size = 2;                         /* two entries stored */
    }

    /* return already computed value if requested */
    if (n < cache_size) return cache[n];

    /* grow cache to hold terms up to n */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= n) new_cap <<= 1;     /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms via the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 2) continue;                   /* a(0) and a(1) already known */

        idx_t k = i - 2;                       /* k = n in the formula above */
        seq_t term_k  = cache[k];
        seq_t term_k1 = cache[k + 1];

        /* coefficients (2 + k² + 3k) and (4 + 2k) */
        seq_t coeff1 = (seq_t)2 + (seq_t)k * (seq_t)k + (seq_t)3 * (seq_t)k;
        seq_t coeff2 = (seq_t)4 + (seq_t)2 * (seq_t)k;

        cache[i] = coeff1 * term_k + coeff2 * term_k1;
    }
    cache_size = n + 1;
    return cache[n];
}
