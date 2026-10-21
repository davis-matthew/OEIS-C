/*
 * A063336  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          63 ).
 * https://oeis.org/A063336
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13. Initial values (n = 2
 *            … 13): a(2) = -1, a(3) = 87, a(4) = 197, a(5) = 305, a(6) = 413,
 *            a(7) = 525, a(8) = 633, a(9) = 741, a(10) = 853, a(11) = 961,
 *            a(12) = 1069, a(13) = 1179
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return a default value for indices below the offset. */
    if (n < 2)
        return (seq_t)0;

    /* Static cache and bookkeeping. */
    static seq_t *cache = NULL;
    static size_t  cache_size   = 0;   /* number of allocated slots (indices 0 … cache_size-1) */
    static idx_t   last_computed = 13; /* largest index whose value is currently computed */

    /* Initial allocation and initial values. */
    if (!cache) {
        cache_size = 14;                   /* indices 0 … 13 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* Ensure the allocation succeeded – if it fails, we fallback to a naïve
           computation for the requested index only (rare case, kept simple). */
        if (!cache)
            return (seq_t)0;

        /* Default all entries to 0. */
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = (seq_t)0;

        /* Initialise the known initial terms. */
        cache[2]  = (seq_t)-1;
        cache[3]  = (seq_t)87;
        cache[4]  = (seq_t)197;
        cache[5]  = (seq_t)305;
        cache[6]  = (seq_t)413;
        cache[7]  = (seq_t)525;
        cache[8]  = (seq_t)633;
        cache[9]  = (seq_t)741;
        cache[10] = (seq_t)853;
        cache[11] = (seq_t)961;
        cache[12] = (seq_t)1069;
        cache[13] = (seq_t)1179;
    }

    /* Ensure the cache can hold index n. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Reallocation failed – compute the requested term on the fly
               and return it (no caching). This fallback keeps the function
               defined for all n, though it may be slower. */
            if (n <= 13)
                return cache[n];
            /* For n > 13, use the recurrence with temporary variables. */
            seq_t a_n_m4 = cache[n - 4];
            seq_t a_n_m6 = cache[n - 6];
            seq_t a_n_m10 = cache[n - 10];
            return a_n_m4 + a_n_m6 - a_n_m10;
        }
        cache = tmp;
        /* Zero-initialize the newly allocated part. */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Compute missing values up to n, using the recurrence. */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* For i <= 13 the values are already in the cache. */
            if (i > 13) {
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
            }
        }
        last_computed = n;
    }

    /* Return the cached result. */
    return cache[(size_t)n];
}
