/*
 * A064343  Generalized Catalan numbers C(5,5; n).
 * https://oeis.org/A064343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   4*(n-1)*a(n) = 5*(79*n-200)*a(n-1) + 250*(2*n-3)*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with known initial terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;   /* a(0) */
        cache[1] = 1;   /* a(1) */
    }

    /* Extend cache up to required index */
    while (cache_len <= n) {
        idx_t i = cache_len;   /* compute a(i) */

        /* Compute the right‑hand side of the recurrence.
           To avoid negative intermediates we branch on the sign of (79*i-200). */
        seq_t rhs;
        if (79ULL * i >= 200ULL) {
            /* RHS = 5*(79*i-200)*a(i-1) + 250*(2*i-3)*a(i-2) */
            rhs = 5ULL * (seq_t)(79ULL * i - 200ULL) * cache[i - 1]
                + 250ULL * (seq_t)(2ULL * i - 3ULL) * cache[i - 2];
        } else {
            /* RHS = 250*(2*i-3)*a(i-2) - 5*(200-79*i)*a(i-1)  */
            rhs = 250ULL * (seq_t)(2ULL * i - 3ULL) * cache[i - 2]
                - 5ULL * (seq_t)(200ULL - 79ULL * i) * cache[i - 1];
        }

        /* Denominator is always positive for i >= 2 */
        seq_t denom = 4ULL * (seq_t)(i - 1ULL);

        /* Exact division, result fits in seq_t for the tested n */
        seq_t val = rhs / denom;

        /* Grow cache and store new value */
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n];
}
