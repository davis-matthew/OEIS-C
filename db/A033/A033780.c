/*
 * A033780  Product t2(q^d); d | 21, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 765, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A033780: a(n) = coeff of q^n in ∏_{d|21} t2(q^d)
 * where t2(q) = θ2(q)/(2q^(1/4)) = Σ_{k≥0} q^{k(k+1)/2}.
 * The product expands as sums of four scaled triangular numbers.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If the requested index is already cached, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* ---------- Prepare to compute all values up to n ---------- */
    const idx_t factors[4] = {1, 3, 7, 21};
    const int num_factors = 4;

    /* Determine maximal triangular index for this n */
    idx_t max_tri = 0;
    while ((max_tri * (max_tri + 1)) / 2 <= n) {
        ++max_tri;
    }

    /* Pre‑compute triangular numbers up to n */
    idx_t *tri = (idx_t*)malloc(max_tri * sizeof(idx_t));
    for (idx_t i = 0; i < max_tri; ++i) {
        tri[i] = (i * (i + 1)) / 2;
    }

    /* Temporary arrays for convolution */
    idx_t size = n + 1;
    seq_t *cur  = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(size, sizeof(seq_t));

    /* cur[0] = 1 represents choosing the zero term from the empty product */
    cur[0] = (seq_t)1;

    /* Convolve with each factor one after another */
    for (int f = 0; f < num_factors; ++f) {
        idx_t d = factors[f];
        memset(next, 0, size * sizeof(seq_t));

        for (idx_t ti = 0; ti < max_tri; ++ti) {
            idx_t t_scaled = d * tri[ti];          /* integer arithmetic, safe */
            if (t_scaled > n) break;

            for (idx_t m = t_scaled; m <= n; ++m) {
                next[m] += cur[m - t_scaled];
            }
        }
        /* swap cur and next for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* ---------- Store computed values in the cache ---------- */
    free(cache);                   /* free any previous cache */
    cache = (seq_t*)malloc(size * sizeof(seq_t));
    memcpy(cache, cur, size * sizeof(seq_t));
    cache_size = size;

    /* Clean up temporary memory */
    free(tri);
    free(cur);
    free(next);

    /* Return the requested value */
    return cache[n];
}
