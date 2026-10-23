/*
 * A011747  Expansion of (1 + x^2 + x^4)/(1 + x^2 + x^3 + x^4 + x^5) mod 2.
 * https://oeis.org/A011747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A011747
   Formula: a(n) = coefficient of x^n in (1 + x^2 + x^4)/(1 + x^2 + x^3 + x^4 + x^5)
            over GF(2).  The coefficients satisfy the linear recurrence
            a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-5) + r(n)  (mod 2)
            for n >= 0, where r(0)=r(2)=r(4)=1 and r(n)=0 otherwise.   */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;   /* allocated number of slots          */
    static idx_t  cache_len  = 0;   /* number of stored terms (>=1)       */

    /* helper: grow cache to at least needed+1 slots */
    if (n >= cache_len) {
        /* ensure storage big enough */
        size_t needed = (size_t)n + 1;
        if (needed > cache_size) {
            size_t newsize = cache_size > 0 ? cache_size : 64;
            while (newsize < needed) newsize <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed – fallback */
            cache = tmp;
            /* zero newly allocated area */
            for (size_t i = cache_size; i < newsize; ++i) cache[i] = 0;
            cache_size = newsize;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term = 0;

            /* recurrence part: a(i-2) + a(i-3) + a(i-4) + a(i-5) */
            if (i >= 2) term ^= cache[(size_t)(i - 2)];
            if (i >= 3) term ^= cache[(size_t)(i - 3)];
            if (i >= 4) term ^= cache[(size_t)(i - 4)];
            if (i >= 5) term ^= cache[(size_t)(i - 5)];

            /* add rhs(i) = 1 when i==0 or i==2 or i==4 */
            if (i == 0 || i == 2 || i == 4) term ^= (seq_t)1;

            cache[(size_t)i] = term;
        }

        cache_len = (idx_t)n + 1;
    }

    return cache[(size_t)n];
}
