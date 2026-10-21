/*
 * A084944  Hendecagorials: n-th polygorial for k=11.
 * https://oeis.org/A084944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, 2*a(n) = n*(9*n-7)*a(n-1) (OEIS A084944)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache is large enough */
    if (n >= cache_len) {
        idx_t new_len = (n + 1) > 1 ? (n + 1) : 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* out of memory, unlikely for valid inputs */

        /* Preserve existing values */
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

        /* Initialise from scratch if cache was empty */
        if (cache_len == 0) {
            new_cache[0] = (seq_t)1;               /* a(0) = 1 */
        }

        /* Compute missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) continue;                  /* already set */
            seq_t t1 = (seq_t)i;
            seq_t t2 = t1 * (((seq_t)9 * t1) - (seq_t)7);
            seq_t prod = t2 / (seq_t)2;            /* division is exact */
            new_cache[i] = new_cache[i - 1] * prod;
        }

        /* Free old cache and replace */
        if (cache) free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
