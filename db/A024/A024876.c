/*
 * A024876  Expansion of (3-2*x-3*x^2-4*x^3)/(1-3*x+x^2+x^3+x^4).
 * https://oeis.org/A024876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3, a(1)=7, a(2)=15, a(3)=31; for n>=4:
 *            a(n)=3*a(n-1)-a(n-2)-a(n-3)-a(n-4)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = (idx_t)-1;      /* last index computed */

    /* initialise cache with the first four terms on the first call */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failed */

        cache[0] = (seq_t)3;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)31;
        last = 3;
    }

    /* already computed: just return */
    if (n <= last) return cache[n];

    /* make sure the buffer is large enough */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t = cache[i-1];
        t = t * (seq_t)3;
        t = t - cache[i-2];
        t = t - cache[i-3];
        t = t - cache[i-4];
        cache[i] = t;
    }
    last = n;

    return cache[n];
}
