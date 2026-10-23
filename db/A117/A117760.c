/*
 * A117760  Expansion of 1/(1 - x - x^3 - x^5 - x^7).
 * https://oeis.org/A117760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-7); Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=8, a(7)=13
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 95, u128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache  = NULL;   /* a[0 … last] */
    static idx_t  cap    = 0;      /* allocated size of cache */
    static idx_t  last   = 0;      /* highest index fully initialized */

    /* ------------------------------------------------------------ */
    /* 1. Initialise cache on first call                           */
    /* ------------------------------------------------------------ */
    if (cache == NULL) {
        cap   = 8;                  /* start with a small block */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();
        memset(cache, 0, cap * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        last = 0;
    }

    /* ------------------------------------------------------------ */
    /* 2. If requested n is already known, return it              */
    /* ------------------------------------------------------------ */
    if (n <= last) return cache[n];

    /* ------------------------------------------------------------ */
    /* 3. Grow cache if necessary                                 */
    /* ------------------------------------------------------------ */
    if (n + 1 > cap) {
        idx_t new_cap = cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        /* initialise newly added slots to zero */
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* ------------------------------------------------------------ */
    /* 4. Compute missing terms up to index n                      */
    /* ------------------------------------------------------------ */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1];
        if (i >= 3) sum += cache[i - 3];
        if (i >= 5) sum += cache[i - 5];
        if (i >= 7) sum += cache[i - 7];
        cache[i] = sum;
    }
    last = n;
    return cache[n];
}
