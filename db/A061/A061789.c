/*
 * A061789  a(n) = Sum_{k=1..n} prime(k)^prime(k).
 * https://oeis.org/A061789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^prime(k), a(1) = 2^2 = 4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0)                /* below offset, value irrelevant */
        return 0;

    /* Static cache of partial sums: cache[0] = a(1), cache[i] = a(i+1) */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* number of terms currently stored */
    static idx_t cache_cap   = 0;   /* allocated capacity */

    /* Grow cache when needed */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap < 16)? 16 : cache_cap + 16;
        if (new_cap < n)
            new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)          /* allocation failed – cannot compute */
            return 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        uint32_t p = get_prime(k);          /* kth prime (k starts at 1) */

        /* Compute p^p iteratively – seq_t multiplication is safe for overflow */
        seq_t pow = 1;
        for (uint32_t i = 0; i < p; ++i)
            pow *= (seq_t)p;

        seq_t sum = (k == 1) ? pow : cache[k - 2] + pow;
        cache[k - 1] = sum;
    }

    cache_len = n;
    return cache[n - 1];
}
