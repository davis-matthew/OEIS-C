/*
 * A090013  Permanent of (0,1)-matrix of size n X (n+d) with d=3 and n-1 zeros
 *          not on a line.
 * https://oeis.org/A090013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*a(n-1) + (n-2)*a(n-2), a(1) = 4, a(2) = 16. This
 *            recurrence generates OEIS A090013: Permanent of a (0,1)-matrix
 *            of size n×(n+3) with n-1 zeros not on a line
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values.
       Index 0 is unused to keep indices 1‑based. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* largest n for which cache[n] is valid */

    /* Handle the trivial small cases first. If n is 1 or 2 we
       need the values 4 and 16 in the cache. */
    if (n == 1) {
        if (cache_size < 1) {
            cache = (seq_t*)realloc(cache, 3 * sizeof(seq_t));
            cache[0] = 0;                  /* unused */
            cache[1] = 4;
            cache[2] = 16;
            cache_size = 2;
        }
        return cache[1];
    }
    if (n == 2) {
        if (cache_size < 2) {
            cache = (seq_t*)realloc(cache, 3 * sizeof(seq_t));
            cache[0] = 0;
            cache[1] = 4;
            cache[2] = 16;
            cache_size = 2;
        }
        return cache[2];
    }

    /* Ensure the cache is large enough for all indices up to n. */
    if (n > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();              /* unrecoverable memory error */
        cache = tmp;

        /* Compute new terms using the recurrence. */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            seq_t term = ((seq_t)(k + 2)) * cache[k - 1]
                       + ((seq_t)(k - 2)) * cache[k - 2];
            cache[k] = term;
        }
        cache_size = n;
    }

    return cache[n];
}
