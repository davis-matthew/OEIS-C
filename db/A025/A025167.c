/*
 * A025167  E.g.f: exp(x/(1-2*x))/(1-2*x).
 * https://oeis.org/A025167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 3, a(n) = (4*n-1)*a(n-1) - 4*(n-1)^2*a(n-2) (n
 *            >= 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of already computed terms   */
    static size_t cap   = 0;      /* allocated capacity in elements    */
    static size_t size  = 0;      /* number of computed elements       */

    /* Grow the cache if necessary */
    if (n + 1 > cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* Compute base cases if not yet computed */
    if (size == 0) {
        cache[0] = (seq_t)1;
        size = 1;
        if (n == 0) return cache[0];
    }
    if (size == 1) {
        cache[1] = (seq_t)3;
        size = 2;
        if (n == 1) return cache[1];
    }

    /* Compute up to n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t i_seq      = (seq_t)i;
        seq_t im1        = (seq_t)(i - 1);
        seq_t coeff      = (seq_t)4 * i_seq - (seq_t)1;           /* 4*i-1   */
        seq_t term1      = coeff * cache[i - 1];
        seq_t term2      = (seq_t)4 * im1 * im1 * cache[i - 2];
        cache[i] = term1 - term2;
    }
    size = n + 1;   /* now all indices 0..n are present */

    return cache[n];
}
