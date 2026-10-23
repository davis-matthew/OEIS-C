/*
 * A131441  Row sums of triangle A130757 (coefficients of scaled
 *          Laguerre-Sonin polynomials n!(2^(n-m))*L(n,1/2,x)).
 * https://oeis.org/A131441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) follows the recurrence * a(n) + 2*(1-2*n)*a(n-1) +
 *            2*(2*n-1)*(n-1)*a(n-2) = 0, * with a(0)=1 and a(1)=2. The
 *            function memoises computed values * so that calls may appear in
 *            any order
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"   /* provides seq_t *alloc_seq(size_t n); */

static seq_t *cache   = NULL;
static size_t cache_alloc = 0;
static idx_t  cache_size  = 0;

seq_t a(idx_t n)
{
    /* ensure cache has enough space for indices up to n */
    if (cache_size == 0) {
        cache_alloc = 2;
        cache = (seq_t *)alloc_seq(cache_alloc);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
    }
    if (n + 1 > cache_alloc) {
        size_t new_alloc = cache_alloc;
        while (n + 1 > new_alloc) new_alloc <<= 1;
        cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!cache) abort();
        cache_alloc = new_alloc;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t i_seq = (seq_t)i;

        /* term1 = -2 * (1 - 2*i) * a(i-1) */
        seq_t mult1 = (seq_t)(-2) * ((seq_t)1 - (seq_t)2 * i_seq);
        seq_t term1 = mult1 * prev1;

        /* term2 = -2 * (2*i - 1) * (i - 1) * a(i-2) */
        seq_t mult2 = (seq_t)(-2) * ((seq_t)(2 * i_seq - 1)) * (i_seq - (seq_t)1);
        seq_t term2 = mult2 * prev2;

        cache[i] = term1 + term2;
    }
    cache_size = n + 1;
    return cache[n];
}
