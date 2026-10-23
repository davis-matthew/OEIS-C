/*
 * A185965  Central coefficients of number triangle A185962.
 * https://oeis.org/A185965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A185962(2n,n) Computed via the recurrence 15*n*(n-1)*a(n)
 *            + 10*(2*n-1)*(n-1)*a(n-1) + (109*n^2-263*n+180)*a(n-2) +
 *            4*(-2*n^2-10*n+57)*a(n-3) + 60*(n-4)^2*a(n-4) -
 *            6*(2*n-7)*(n-5)*a(n-5) = 0 Initial values: a(0)=1, a(1)=-2,
 *            a(2)=0, a(3)=8, a(4)=-10
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 15, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for n = 0..4 */
    static const seq_t base[5] = {1, -2, 0, 8, -10};

    /* Cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index computed + 1 */
    static idx_t cache_alloc = 0;  /* allocated size */

    if (n < 5) {
        return base[(idx_t)n];
    }

    /* Allocate or grow cache */
    if (cache == NULL) {
        cache_alloc = (idx_t)16;
        if (cache_alloc < n + 1) cache_alloc = n + 1;
        cache = (seq_t*)malloc(cache_alloc * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i) {
            cache[i] = base[i];
        }
        cache_size = 5;
    } else if (n + 1 > cache_alloc) {
        idx_t new_alloc = cache_alloc * 2;
        if (new_alloc < n + 1) new_alloc = n + 1;
        cache = (seq_t*)realloc(cache, new_alloc * sizeof(seq_t));
        cache_alloc = new_alloc;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t nseq = (seq_t)i;

        seq_t term1 = 10 * (2 * nseq - 1) * (nseq - 1) * cache[i - 1];

        seq_t coeff2 = 109 * nseq * nseq - 263 * nseq + 180;
        seq_t term2 = coeff2 * cache[i - 2];

        seq_t coeff3 = 4 * (-2 * nseq * nseq - 10 * nseq + 57);
        seq_t term3 = coeff3 * cache[i - 3];

        seq_t coeff4 = 60 * (nseq - 4) * (nseq - 4);
        seq_t term4 = coeff4 * cache[i - 4];

        seq_t coeff5 = -6 * (2 * nseq - 7) * (nseq - 5);
        seq_t term5 = coeff5 * cache[i - 5];

        seq_t others = term1 + term2 + term3 + term4 + term5;
        seq_t denom = 15 * nseq * (nseq - 1);

        cache[i] = -others / denom;
    }

    cache_size = n + 1;
    return cache[n];
}
