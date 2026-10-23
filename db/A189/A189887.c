/*
 * A189887  Dimension of homogeneous component of degree n in x in the
 *          Malcev-Poisson superalgebra S^tilde(M).
 * https://oeis.org/A189887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4) for n > 7, * with
 *            initial values: a(1)=1, a(2)=1, a(3)=2, a(4)=3, * a(5)=4,
 *            a(6)=6, a(7)=9. The values are computed * incrementally and
 *            cached for efficiency
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached table of already computed terms */
    static seq_t *cache     = NULL;   /* array storing a(1) .. a(cached_n) */
    static idx_t cache_len  = 0;      /* allocated length of the array */
    static idx_t cached_n   = 0;      /* number of terms that have been filled */

    /* Zero index is unused; we use 0-based indexing internally. */
    if (n == 0) return (seq_t)0; /* safe guard, not part of sequence */

    /* increase allocation if needed */
    if (cache_len < n) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – shouldn't happen */
        cache     = tmp;
        cache_len = new_len;
    }

    /* if we already have the required term, return it */
    if (cached_n >= n) {
        return cache[n-1];
    }

    /* initial fixed values up to a(7) */
    static const seq_t init[7] = {
        (seq_t)1, /* a(1) */
        (seq_t)1, /* a(2) */
        (seq_t)2, /* a(3) */
        (seq_t)3, /* a(4) */
        (seq_t)4, /* a(5) */
        (seq_t)6, /* a(6) */
        (seq_t)9  /* a(7) */
    };

    idx_t i;
    for (i = cached_n + 1; i <= n && i <= 7; ++i) {
        cache[i-1] = init[i-1];
        cached_n = i;
    }

    if (cached_n >= n) {
        return cache[n-1];
    }

    /* recurrence for n >= 8:
     * a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4)
     */
    idx_t start = (cached_n < 8) ? 8 : cached_n + 1;
    for (i = start; i <= n; ++i) {
        size_t idx = (size_t)(i-1);
        cache[idx] = 2 * cache[idx-1]
                   - 2 * cache[idx-2]
                   + 2 * cache[idx-3]
                   -  cache[idx-4];
    }
    cached_n = n;
    return cache[n-1];
}
