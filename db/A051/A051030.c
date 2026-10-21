/*
 * A051030  Ramanujan's c-series: expansion of
 *          (2+8*x-10*x^2)/(1-82*x-82*x^2+x^3).
 * https://oeis.org/A051030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 82*a(n-1) + 82*a(n-2) - a(n-3) with initial values
 *            a(0)=2, a(1)=172, a(2)=14258
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cache.h"   /* provides seq_t *alloc_cache(idx_t n) */

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *seq    = NULL;      /* array of computed terms */
    static idx_t last    = (idx_t)-1;  /* index of last computed term */
    /* we only need to support indices up to about 10000 in the tests */
    static const idx_t MAX_N = 10001;  /* capacity (0‑based) */

    /* Allocate the cache on first use */
    if (seq == NULL) {
        seq = alloc_cache(MAX_N);      /* allocates an array of seq_t of size MAX_N */
        /* initialise the first three terms */
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)172;
        seq[2] = (seq_t)14258;
        last   = 2;
    }

    /* Compute any missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq[i] = (seq_t)82 * seq[i-1] + (seq_t)82 * seq[i-2] - seq[i-3];
        }
        last = n;
    }

    /* For n < 3 the array already holds the correct values */
    return seq[n];
}
