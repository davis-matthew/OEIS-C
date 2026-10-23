/*
 * A155084  A Catalan transform of [x^n](1/(1-2x-2x^2)) (A002605).
 * https://oeis.org/A155084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   3*n*a(n)+6*(3-4*n)*a(n-1)+4*(11*n-18)*a(n-2)+8*(2*n-3)*a(n-3)=0
 *            * initial values: a(0)=1, a(1)=2, a(2)=8
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The sequence element type and index type are supplied by the build. */
seq_t a(idx_t n)
{
    /* static cache holding previously computed terms */
    static seq_t *cache     = NULL;   /* array of cached values      */
    static size_t cache_len = 0;     /* number of valid terms       */
    static size_t cache_cap = 0;     /* allocated capacity (in terms) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 3;                     /* need at least a(0)..a(2)   */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache_len = 3;
    }

    /* Grow the cache if we need more terms */
    if ((size_t)n >= cache_len) {
        /* Determine new capacity (simple strategy: grow to n+1) */
        size_t required = (size_t)n + 1;
        if (required > cache_cap) {
            /* Allocate larger buffer */
            seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
            for (size_t i = 0; i < cache_len; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
            cache = new_cache;
            cache_cap = required;
        }
        /* Compute missing terms up to n */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            /* recurrence coefficients */
            idx_t four_i_minus_3   = 4 * i - 3;
            idx_t eleven_i_minus_18 = 11 * i - 18;
            idx_t two_i_minus_3    = 2 * i - 3;

            /* terms in the numerator */
            seq_t term1 = (seq_t)6 * (seq_t)four_i_minus_3 * cache[i - 1];
            seq_t term2 = (seq_t)4 * (seq_t)eleven_i_minus_18 * cache[i - 2];
            seq_t term3 = (seq_t)8 * (seq_t)two_i_minus_3    * cache[i - 3];

            seq_t numerator = term1 - term2 - term3;
            seq_t denom     = (seq_t)3 * (seq_t)i;

            cache[i] = numerator / denom;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
