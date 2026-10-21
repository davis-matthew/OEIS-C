/*
 * A216779  Number of derangements on n elements with an odd number of cycles.
 * https://oeis.org/A216779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0; a(n+1)= n*(a(n)+a(n-1)-n+2)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the OEIS sequence A216779: Number of derangements on n
   elements with an odd number of cycles.  The recurrence used is
   a(n+1) = n*(a(n)+a(n-1)-n+2).  Results are cached so that
   repeated calls (in any order) are fast. */
seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache     = NULL;   /* array of a(i) */
    static idx_t   cache_cap = 0;     /* allocated size */
    static idx_t   cache_max = 0;     /* highest index computed */

    /* Ensure cache is at least two elements (a[0] and a[1]) */
    if (cache_cap == 0) {
        cache_cap = (n >= 1 ? n + 1 : 2);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;             /* out of memory: small fallback */
        cache[0] = 0;
        if (cache_cap > 1) cache[1] = 0;
        cache_max = (cache_cap > 1 ? 1 : 0);
    }

    /* Grow cache if requested index is beyond current capacity */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;              /* out of memory: small fallback */
        cache_cap = new_cap;
    }

    /* Compute values up to n if not yet done */
    if (n > cache_max) {
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            /* k >= 2 here */
            idx_t g = k - 1;
            seq_t tmp = cache[k - 1] + cache[k - 2] + (seq_t)2;
            tmp -= (seq_t)g;                    /* tmp = a(k-1)+a(k-2)+2 - (k-1) */
            cache[k] = (seq_t)g * tmp;
        }
        cache_max = n;
    }

    return cache[n];
}
