/*
 * A166753  Partial sums of A166752.
 * https://oeis.org/A166753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A166753: Partial sums of A166752
 *
 * Recurrence:
 *     a(0)=1, a(1)=2, a(2)=5, a(3)=6, a(4)=17
 *     a(n) = a(n-1) + 5*a(n-2) - 5*a(n-3) - 4*a(n-4) + 4*a(n-5)   (n≥5)
 *
 * The function caches computed values so that subsequent calls with
 * arbitrary n are O(1) after the initial computation.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;   /* number of cached values */
    static size_t cache_cap = 0;   /* capacity of the cache array */

    size_t idx = (size_t)n;   /* cast once for array indexing */

    /* If the result is already cached, return it immediately */
    if (idx < cache_len) {
        return cache[idx];
    }

    /* Resize cache if needed */
    if (idx + 1 > cache_cap) {
        size_t new_cap = idx + 1;   /* exact size needed */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure: unlikely */
            return 0;
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val;
        if (i == 0)           val = 1;
        else if (i == 1)      val = 2;
        else if (i == 2)      val = 5;
        else if (i == 3)      val = 6;
        else if (i == 4)      val = 17;
        else {
            /* Use only seq_t arithmetic to avoid overflow issues */
            val = cache[i - 1]
                + ((seq_t)5 * cache[i - 2])
                - ((seq_t)5 * cache[i - 3])
                - ((seq_t)4 * cache[i - 4])
                + ((seq_t)4 * cache[i - 5]);
        }
        cache[i] = val;
    }

    cache_len = idx + 1;
    return cache[idx];
}
