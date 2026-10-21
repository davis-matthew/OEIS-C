/*
 * A052700  Expansion of e.g.f. x*(1-x)/(1-3*x).
 * https://oeis.org/A052700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^(n-2)*n! for n >= 2 Initial terms: a(0)=0, a(1)=1,
 *            a(2)=4 (verified 0,1,4,36,432,…) Recurrence: a(n)=3*n*a(n-1) for
 *            n >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- Internal cache for repeated calls -------------------------------- */
static seq_t *cache = NULL;        /* array of computed terms            */
static idx_t  cache_cnt = 0;      /* number of elements initially set   */
static idx_t  cache_cap = 0;      /* current allocated capacity         */

/* Ensure that the cache holds terms up to index 'upto' (inclusive) */
static void ensure_cache(idx_t upto)
{
    /* Grow capacity if required */
    if ((idx_t)(upto + 1) > cache_cap) {
        idx_t new_cap = (idx_t)(upto + 1);
        seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_buf) return;                     /* allocation failure – silently fail */
        for (idx_t i = 0; i < cache_cnt; ++i)
            new_buf[i] = cache[i];
        if (cache) free(cache);
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* Initialise missing base values (only once) */
    if (cache_cnt == 0) {
        cache[0] = 0;
        cache_cnt = 1;
    }
    if (cache_cnt <= 1 && upto >= 1) {
        cache[1] = 1;
        cache_cnt = 2;
    }
    if (cache_cnt <= 2 && upto >= 2) {
        cache[2] = 4;
        cache_cnt = 3;
    }

    /* Compute from the first missing element up to 'upto' */
    for (idx_t i = cache_cnt; i <= upto; ++i) {
        seq_t factor = (seq_t)i * 3;     /* 3 * i  (seq_t multiplication)   */
        cache[i] = cache[i - 1] * factor;
    }
    cache_cnt = upto + 1;                /* we now have terms 0 … upto      */
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) {                      /* handle the first three terms */
        if (n == 0) return 0;
        if (n == 1) return 1;
        return 4;                     /* n == 2 */
    }

    ensure_cache(n);                  /* compute / reuse cached value */
    return cache[n];
}
