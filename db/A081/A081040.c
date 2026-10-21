/*
 * A081040  5th binomial transform of (1,4,0,0,0,0,...).
 * https://oeis.org/A081040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 25*a(n-2), a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* The types seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of entries already computed */

    /* Ensure base cases are in place */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) {                     /* allocation failure: fall back to constants */
            if (n == 0) return (seq_t)1;
            if (n == 1) return (seq_t)9;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
    }

    /* For n = 0 or 1 we already know the values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failure: compute only what we have */
            if (n < cache_len) return cache[n];
            /* Otherwise just compute on the fly using recurrence without cache */
            seq_t a_nm2 = cache[0];
            seq_t a_nm1 = cache[1];
            seq_t a_n;

            for (size_t i = 2; i <= (size_t)n; ++i) {
                a_n = a_nm1 * (seq_t)10 - a_nm2 * (seq_t)25;
                a_nm2 = a_nm1;
                a_nm1 = a_n;
            }
            return a_n;
        }
        cache = tmp;
        /* Compute new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            cache[i] = cache[i-1] * (seq_t)10 - cache[i-2] * (seq_t)25;
        }
        cache_len = new_len;
    }
    return cache[n];
}
