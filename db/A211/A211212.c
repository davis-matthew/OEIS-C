/*
 * A211212  4-alternating permutations of length 4n.
 * https://oeis.org/A211212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence:  a(0) = 1;
 *              a(n) = sum_{k=1..n} (-1)^(k+1) * binomial(4*n,4*k) * a(n-k)
 *   where binomial is the standard binomial coefficient function.
 */
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t next_compute = 0;      /* first index that has not yet been computed */

    /* make sure the cache has room for indices up to n */
    if (n >= next_compute)
        ensure_cache(n, &cache, &cap);

    /* compute any missing values up to n inclusively */
    for (idx_t i = next_compute; i <= n; ++i)
    {
        if (i == 0)                /* a(0) = 1 */
        {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;

        for (idx_t k = 1; k <= i; ++k)
        {
            /* binomial coefficient binom(4*i,4*k) */
            seq_t bin = binomial((idx_t)(4*i), (idx_t)(4*k));

            /* term = a[i-k] * bin */
            seq_t term = cache[i-k] * bin;

            /* alternating sign: + for odd k, - for even k */
            if (k & 1)
                sum_pos += term;
            else
                sum_neg += term;
        }

        /* final value: sum_pos - sum_neg (sum_pos >= sum_neg for all n) */
        cache[i] = sum_pos - sum_neg;
    }

    /* next_compute now points to the first index still uncomputed */
    next_compute = n + 1;

    return cache[n];
}
