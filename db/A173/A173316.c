/*
 * A173316  a(n) = 6*n! - 1.
 * https://oeis.org/A173316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + (n-1), a(0) = 5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 6*n! - 1  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache contains at least n+1 entries */
    if (cache_len <= n) {
        /* New size = n+1 entries */
        idx_t new_len = n + 1;
        /* Reallocate (or allocate if first time) */
        cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!cache) {
            /* Allocation failed – return 0 as fallback */
            return (seq_t)0;
        }
        /* Compute missing values starting from current length */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 5;
            } else {
                /* cache[i] = i * cache[i-1] + (i-1) */
                cache[i] = (seq_t)i * cache[i - 1] + ((seq_t)(i - 1));
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
