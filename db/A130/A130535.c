/*
 * A130535  a(0)=a(1)=1; a(n+2) = a(floor((n+1)/a(n+1))) + a(floor(n/a(n))).
 * https://oeis.org/A130535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 405, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Oeis sequence A130535:
   a(0) = 1; a(1) = 1;
   a(n+2) = a( floor((n+1)/a(n+1)) ) + a( floor(n/a(n)) )
   (for n ≥ 0)  */
#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"   /* ensures cache allocation */

seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t  cap   = 0;        /* current allocated length */
    static idx_t   last  = 1;        /* highest index computed so far */

    /* initialise cache with a[0] and a[1] on first call */
    if (!cache) {
        ensure_cache(1, &cache, &cap);   /* allocate at least two elements */
        cache[0] = 1;
        cache[1] = 1;
    }

    /* return directly for small indices */
    if (n <= 1) {
        return 1;
    }

    /* grow cache to hold indices up to n */
    if (n >= cap) {
        ensure_cache(n, &cache, &cap);
    }

    /* compute missing values in ascending order */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* i corresponds to n+2 in the recurrence, so we need
           a[i-1] and a[i-2] which are already computed. */
        idx_t idx1 = (idx_t)(((seq_t)(i - 1)) / cache[i - 1]);  /* floor((i-1)/a[i-1]) */
        idx_t idx2 = (idx_t)(((seq_t)(i - 2)) / cache[i - 2]);  /* floor((i-2)/a[i-2]) */
        cache[i] = cache[idx1] + cache[idx2];
    }

    /* update bookkeeping */
    if (n > last) {
        last = n;
    }

    return cache[n];
}
