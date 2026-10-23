/*
 * A176848  Number of compositions of n into floor(j/3) kinds of j's for all
 *          j>=1.
 * https://oeis.org/A176848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=a(n-1)+2*a(n-3)-a(n-4) for n>=5 Initial values: a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=1
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values */
    static const seq_t base[5] = {1, 0, 0, 1, 1};

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* allocated capacity */
    static idx_t max_comp = 0;       /* highest index computed so far */

    /* handle trivial small indices */
    if (n < 5)
        return base[n];

    /* initialise cache once */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        max_comp = 4;
    }
    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap > 0 ? cap : 8;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute up to requested n */
    if (max_comp < n) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            /* a(i) = a(i-1) + 2*a(i-3) - a(i-4) */
            seq_t v1 = cache[i - 1];
            seq_t v3 = cache[i - 3];
            seq_t v4 = cache[i - 4];
            seq_t val = v1 + v3 + v3 - v4;   /* v3 added twice = 2*v3 */
            cache[i] = val;
        }
        max_comp = n;
    }

    return cache[n];
}
