/*
 * A082177  Professor Umbugio's sequence A082176 divided by 1946.
 * https://oeis.org/A082177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7266*a(n-1) - 19690571*a(n-2) + 23585007306*a(n-3) -
 *            10533473613720*a(n-4) initial values: a(0)=0, a(1)=0, a(2)=106,
 *            a(3)=577647
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* length (N+1) of allocated array */
    static idx_t max_computed = 0;/* highest index that has been computed */

    /* grow the cache if needed */
    if (n >= cache_len) {
        /* allocate new larger array */
        seq_t *tmp = seq_alloc_new_array((size_t)n + 1);
        if (!tmp) return (seq_t)0;  /* allocation failure – unlikely */

        /* copy existing values */
        if (cache && cache_len > 0)
            memcpy(tmp, cache, cache_len * sizeof(seq_t));

        /* free old array */
        if (cache)
            seq_free_array(cache);

        cache = tmp;
        cache_len = (size_t)n + 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)0;
        } else if (i == 2) {
            cache[i] = (seq_t)106;
        } else if (i == 3) {
            cache[i] = (seq_t)577647;
        } else {
            /* recurrence: a(n) = 7266*a(n-1) - 19690571*a(n-2)
                                   + 23585007306*a(n-3) - 10533473613720*a(n-4) */
            seq_t t1 = cache[i-1] * (seq_t)7266U;
            seq_t t2 = cache[i-2] * (seq_t)19690571U;
            seq_t t3 = cache[i-3] * (seq_t)23585007306ULL;
            seq_t t4 = cache[i-4] * (seq_t)10533473613720ULL;

            /* compute (t1 + t3) - (t2 + t4) to avoid negative intermediates */
            cache[i] = (t1 + t3) - (t2 + t4);
        }
    }

    if (n > max_computed)
        max_computed = n;

    return cache[n];
}
