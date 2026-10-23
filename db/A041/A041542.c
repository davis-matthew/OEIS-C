/*
 * A041542  Numerators of continued fraction convergents to sqrt(288).
 * https://oeis.org/A041542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 16, a(1) = 17 For n ≥ 2: if n is odd : a(n) = a(n-1) +
 *            a(n-2) if n is even : a(n) = 32 * a(n-1) + a(n-2)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache     = NULL;   /* array of computed a(k) */
    static idx_t  cache_sz  = 0;      /* allocated array size          */
    static idx_t  cache_len = 0;      /* number of entries computed   */

    /* last two computed values – used only when extending the cache */
    static seq_t prev_prev = 0;
    static seq_t prev      = 0;

    /* --- initialisation ------------------------------------------------- */
    if (cache_len == 0) {
        /* allocate space for a(0) and a(1) */
        cache_sz  = 2;
        cache     = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed – unlikely */

        cache[0] = (seq_t)16;
        cache[1] = (seq_t)17;
        cache_len = 2;

        prev_prev = cache[0];
        prev      = cache[1];
    }

    /* --- make sure the array is large enough ----------------------------- */
    if ((n + 1) > cache_sz) {
        idx_t new_sz = cache_sz;
        while (new_sz < (n + 1)) new_sz *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* allocation failed – unlikely */
        cache     = tmp;
        cache_sz  = new_sz;
    }

    /* --- extend the cache to include a(n) if necessary ------------------ */
    if ((n + 1) > cache_len) {
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t cur;
            if (k & 1U) {             /* k odd – partial quotient = 1  */
                cur = prev + prev_prev;
            } else {                  /* k even – partial quotient = 32 */
                cur = prev * (seq_t)32 + prev_prev;
            }
            cache[k] = cur;
            /* shift for next iteration */
            prev_prev = prev;
            prev      = cur;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
