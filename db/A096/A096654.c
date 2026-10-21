/*
 * A096654  Denominators of self-convergents to 1/(e-2).
 * https://oeis.org/A096654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (n+1)*a(n-1) + n*a(n-2) with a(0) = 1, a(1) = 2. This is
 *            the recurrence relation for A096654. The function memoises
 *            previously computed terms so that arbitrary order and repeated
 *            calls are efficient. It is O(n) per call for the largest
 *            requested n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of elements currently stored */

    /* If nothing is cached yet, initialise up to the requested index
       (or at least two elements for the base cases). */
    if (cache_len == 0) {
        cache_len = (n >= 1) ? (n + 1) : 2;   /* at least two entries */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;                  /* out of memory → 0 */

        cache[0] = 1;
        if (cache_len > 1) cache[1] = 2;
        if (cache_len > 2) {
            idx_t i;
            for (i = 2; i < cache_len; ++i) {
                cache[i] = ((seq_t)(i + 1)) * cache[i - 1]
                         + ((seq_t)i) * cache[i - 2];
            }
        }
        return cache[n];
    }

    /* Extend cache if the requested index is beyond the current range */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;  /* out of memory → 0 */
        cache = new_cache;
        idx_t i;
        for (i = cache_len; i <= n; ++i) {
            /* Base cases are already set; only compute from i = cache_len */
            cache[i] = ((seq_t)(i + 1)) * cache[i - 1]
                     + ((seq_t)i) * cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
