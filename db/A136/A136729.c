/*
 * A136729  E.g.f.: A(x) = [ exp(x)/(5 - 4*exp(x)) ]^(1/5).
 * https://oeis.org/A136729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A136729
   Recurrence: a(0)=1, a(1)=1,
   a(n) = 1 + 4 * Σ_{k=1}^{n-1} (C(n,k)-1) * a(k)
   where C(n,k) is the binomial coefficient.
   This implementation uses memoisation and computes values on demand.
   The algorithm is O(n²) time and O(n) space.
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated capacity */
    static idx_t cached_upto = 0;   /* one past last computed index */

    /* initialise cache if needed */
    if (cache == NULL) {
        cache_size = n + 1;
        if (cache_size < 2) cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;        /* a(0) */
        cached_upto = 1;            /* next index to compute */
    }

    /* grow cache if requested n exceeds current capacity */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size;
        while ((size_t)n + 1 > new_size) new_size *= 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();
        cache_size = new_size;
    }

    /* compute missing values up to n */
    for (idx_t i = cached_upto; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;                    /* C(i,0) */
        for (idx_t k = 1; k < i; ++k) {     /* loop k = 1 .. i-1 */
            /* binom = C(i,k) computed from C(i,k-1) */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            seq_t term = (binom - (seq_t)1) * cache[k];
            sum += term;
        }
        cache[i] = (seq_t)1 + (seq_t)4 * sum;
    }
    cached_upto = n + 1;

    return cache[n];
}
