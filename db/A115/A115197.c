/*
 * A115197  Convolution of generalized Catalan numbers A064062 (called
 *          C(n;2)).
 * https://oeis.org/A115197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2 * a(n) = ((7*n - 13)*a(n-1) + 4*(2*n -
 *            1)*a(n-2)) / n for n >= 2 * (OEIS A115197: row sums of triangle
 *            A115193)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* static cache storing already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries allocated (>=1) */

    /* grow cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed; caller will handle */

        /* copy existing values (if any) */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* initialise new slots to zero */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;

        /* free old cache if it existed */
        if (cache) free(cache);

        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise seeds if not present */
    if (cache_size >= 1) cache[0] = 1;
    if (cache_size >= 2) cache[1] = 2;

    /* compute all missing values up to n using the recurrence */
    for (idx_t k = 2; k <= n; ++k) {
        /* Use 128‑bit arithmetic for intermediate values to avoid overflow
           when seq_t is 64‑bit and the term still fits. */
        __int128 coef1 = (7 * (idx_t)k) - 13;          /* 7k-13 */
        __int128 coef2 = 4 * ((2 * (idx_t)k) - 1);     /* 4*(2k-1) */
        __int128 prod1 = coef1 * (__int128)cache[k-1];
        __int128 prod2 = coef2 * (__int128)cache[k-2];
        __int128 sum = prod1 + prod2;
        __int128 div = sum / (idx_t)k;                  /* exact division */

        cache[k] = (seq_t)div;
    }

    return cache[n];
}
