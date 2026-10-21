/*
 * A095000  E.g.f.: exp(x)/(1-x)^4.
 * https://oeis.org/A095000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5, a(n) = (n+4)*a(n-1) - (n-1)*a(n-2) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A095000.  The function
   memoises results so that arbitrary‑ordered calls are efficient. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation table, cache[i] = a(i) */
static idx_t  cache_last = 0; /* highest index that has been computed */

/* grow the cache to hold at least new_cap elements (indices 0 .. new_cap-1) */
static void grow_cache(idx_t new_cap) {
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    } else if (new_cap > cache_last + 1) {
        cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    }
    /* initialise new entries to 0 (they will be overwritten when computed) */
    if (cache != NULL) {
        for (idx_t i = cache_last + 1; i < new_cap; ++i)
            cache[i] = 0;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    if (cache == NULL) {
        grow_cache(n + 1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache_last = 1;
    }

    if (n > cache_last) {
        /* compute new terms up to n */
        grow_cache(n + 1);
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t a_im1 = cache[i - 1];
            seq_t a_im2 = cache[i - 2];
            seq_t term1 = (seq_t)(i + 4) * a_im1;
            seq_t term2 = (seq_t)(i - 1) * a_im2;
            cache[i] = term1 - term2;  /* term1 > term2 for n ≥ 2 */
        }
        cache_last = n;
    }

    return cache[n];
}
