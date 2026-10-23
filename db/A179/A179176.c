/*
 * A179176  Number of vertices with even distance from the root in "0-1-2"
 *          Motzkin trees on n edges.
 * https://oeis.org/A179176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence with initial values a(0)=1, a(1)=1, a(2)=3, a(3)=9
 *            3*(n+2)*(2*n-1)*a(n) = (4*n+5)*(2*n-1)*a(n-1) +
 *            (20*n^2+8*n-27)*a(n-2) + 3*(2*n+3)*(4*n-3)*a(n-3) +
 *            9*(2*n+3)*(n-1)*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;         /* pointer to array of seq_t */
    static size_t cache_size = 0;       /* allocated array size (in elements) */
    static size_t cache_len  = 0;       /* number of values currently stored */

    /* initial known values */
    const seq_t init[4] = { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)9 };

    /* Ensure the cached array can hold index n */
    if (n + 1ULL > cache_size) {
        size_t new_size = n + 1ULL;        /* at least n+1 elements */
        if (new_size < 4) new_size = 4;    /* at least 4 slots for init */
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure – unlikely */

        /* copy existing data */
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache      = new_cache;
        cache_size = new_size;
    }

    /* Initialize the first four entries once */
    if (cache_len == 0) {
        size_t limit = (cache_size < 4) ? cache_size : 4;
        for (size_t i = 0; i < limit; ++i)
            cache[i] = init[i];
        cache_len = limit;
    }

    /* If needed, compute up to n */
    if (n + 1ULL > cache_len) {
        /* Working values as idx_t for intermediate coefficients */
        for (size_t i = cache_len; i <= n; ++i) {
            idx_t nn = (idx_t)i;

            /* term1 = (4*n+5)*(2*n-1)*a(n-1) */
            idx_t coeff1_a = 4*nn + 5;
            idx_t coeff1_b = 2*nn - 1;
            seq_t term1 = (seq_t)coeff1_a * (seq_t)coeff1_b * cache[nn-1];

            /* term2 = (20*n^2+8*n-27)*a(n-2) */
            idx_t coeff2 = 20*nn*nn + 8*nn - 27;
            seq_t term2 = (seq_t)coeff2 * cache[nn-2];

            /* term3 = 3*(2*n+3)*(4*n-3)*a(n-3) */
            idx_t coeff3_a = 2*nn + 3;
            idx_t coeff3_b = 4*nn - 3;
            seq_t term3 = (seq_t)3 * (seq_t)coeff3_a * (seq_t)coeff3_b * cache[nn-3];

            /* term4 = 9*(2*n+3)*(n-1)*a(n-4) */
            idx_t coeff4_a = 2*nn + 3;
            idx_t coeff4_b = nn - 1;
            seq_t term4 = (seq_t)9 * (seq_t)coeff4_a * (seq_t)coeff4_b * cache[nn-4];

            /* numerator = term1 + term2 + term3 + term4 */
            seq_t numerator = term1 + term2 + term3 + term4;

            /* denominator = 3*(n+2)*(2*n-1) */
            seq_t denominator = (seq_t)3 * (seq_t)(nn + 2) * (seq_t)(2*nn - 1);

            cache[i] = numerator / denominator;   /* exact division */
        }
        cache_len = n + 1ULL;
    }

    return cache[n];
}
