/*
 * A169298  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a[n] - 43·(a[n-1]+…+a[n-28]) + 22·a[n-29] = c(n)
 *            where c(0)=1, c(1)…c(28)=2, c(29)=1, and c(n)=0 for n≥30
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t   len   = 0;   /* number of elements stored (>=1) */
    static size_t  cap   = 0;   /* allocated capacity */

    /* initialise cache */
    if (!cache) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        len   = 1;
    }

    /* grow capacity if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        idx_t limit = (i >= 28) ? 28 : (size_t)i;
        seq_t sum = 0;
        for (idx_t j = 1; j <= limit; ++j) sum += cache[i - j];

        seq_t term29 = (i >= 29) ? cache[i - 29] : (seq_t)0;

        seq_t p = 0;
        if (i <= 28)          p = (seq_t)2;
        else if (i == 29)     p = (seq_t)1;

        cache[i] = 43 * sum - 22 * term29 + p;
    }
    len = n + 1;

    return cache[n];
}
