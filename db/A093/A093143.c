/*
 * A093143  Expansion of (1-5*x)/(1-10*x).
 * https://oeis.org/A093143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5, a(n) = 10 * a(n-1) for n > 1 (i.e.
 *            a(n)=5·10^(n−1) for n≥1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches values so calls in any order remain O(1) amortised. */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static cache to hold previously computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t   cap   = 0;    /* current allocated capacity */
    static size_t   len   = 0;    /* number of terms stored (valid indices 0 .. len-1) */

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cap = 4;                 /* start with room for a few terms to reduce reallocs */
        len = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {     /* should never happen in the test harness */
            abort();
        }
        cache[0] = (seq_t)1;     /* a(0) */
        cache[1] = (seq_t)5;     /* a(1) */
    }

    /* if already computed, return immediately */
    if (n < len) {
        return cache[n];
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        /* a(i) = 10 * a(i-1) for i >= 2, a(1) already stored */
        seq_t val = cache[i-1] * (seq_t)10;

        /* grow cache if necessary */
        if (i >= cap) {
            cap = (cap == 0) ? 4 : cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (new_cache == NULL) {
                abort();
            }
            cache = new_cache;
        }

        cache[i] = val;
    }

    len = n + 1;        /* we now have terms up to n inclusive */
    return cache[n];
}
