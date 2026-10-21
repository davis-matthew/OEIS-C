/*
 * A094822  Expansion of e.g.f.: exp(3*x)/(1-3*x)^(1/3).
 * https://oeis.org/A094822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = recurrence a(0) = 1, a(1) = 4, a(n) = (3*n + 1)*a(n-1) -
 *            9*(n-1)*a(n-2) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Initialise cache if this is the first call. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }

    /* Extend cache up to index n if necessary. */
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;         /* allocation failure */
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t1 = ((seq_t)3 * (seq_t)i + (seq_t)1) * cache[i - 1];
            seq_t t2 = (seq_t)9 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = t1 - t2;
        }
        cache_len = new_len;
    }

    return cache[n];
}
