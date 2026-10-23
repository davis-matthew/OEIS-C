/*
 * A181560  a(n+1) = a(n-1) + 2 a(n-2) - a(n-4) ; a(0)=1, a(n)=0 for 0 < n <
 *          5.
 * https://oeis.org/A181560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n+1) = a(n-1) + 2*a(n-2) - a(n-4); a(0)=1, a(n)=0 for 0 < n <
 *            5 a(n) is the n‑th term of OEIS A181560
 * Range:     i8 n <= 20, i16 n <= 36, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Array indices are size_t */
    size_t idx = (size_t)n;
    size_t needed = idx + 1;

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;     /* number of computed elements */
    static size_t cache_cap  = 0;     /* current capacity */

    /* Grow the cache if the required element is beyond the current capacity */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? 2 * cache_cap : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* allocation failure */
        if (cache) {                            /* copy existing values */
            for (size_t i = 0; i < cache_len; ++i)
                tmp[i] = cache[i];
            free(cache);
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Initialise base values on the first call */
    if (cache_len == 0) {
        /* cache_cap is already >= 5 from the allocation above */
        cache[0] = (seq_t)1;
        for (size_t i = 1; i < 5; ++i)
            cache[i] = (seq_t)0;
        cache_len = 5;       /* keep at least the five base terms */
    }

    /* Extend the cache up to the requested index using the recurrence
       valid for all indices i >= 5.  cache_len is guaranteed to be >= 5
       at this point. */
    if (needed > cache_len) {
        for (size_t i = cache_len; i < needed; ++i) {
            cache[i] = cache[i-2] + (seq_t)2 * cache[i-3] - cache[i-5];
        }
        cache_len = needed;
    }

    return cache[idx];
}
