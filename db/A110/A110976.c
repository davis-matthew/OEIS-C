/*
 * A110976  Sequence of numerators associated with the continued fraction
 *          based on the sequence d(n)= distance of n from closest prime (
 *          A051699).
 * https://oeis.org/A110976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the convergent of the continued fraction *
 *            with partial quotients d(k) where d(k) = distance from k to the
 *            closest * prime. Recurrence: a(-1)=1, a(0)=2,
 *            a(n)=d(n)*a(n-1)+a(n-2) for n≥1
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime.h"

/* Return the numerator a(n) for the OEIS sequence A110976.  */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;        /* array of computed a(k) */
    static idx_t cached_size = 0;      /* number of terms stored = cached_size */

    /* If we already know the answer, return it. */
    if (n < cached_size)
        return cache[n];

    /* Allocate / reallocate the cache to hold terms up to n. */
    idx_t new_size = n + 1;
    if (cache == NULL)
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* Compute new terms, extending the cache. */
    for (idx_t k = cached_size; k <= n; ++k)
    {
        seq_t dk;           /* distance d(k) to closest prime */

        if (k == 0) {       /* d(0) = 2, but numerator a(0)=2 */
            cache[k] = (seq_t)2;
            continue;
        }
        if (k == 1) {       /* d(1) = 1, a(1)=3 */
            cache[k] = (seq_t)3;
            continue;
        }

        /* Compute d(k) = min( |k - p_low|, |p_high - k| ) */
        uint64_t p_low = 0;     /* largest prime ≤ k (0 if none) */
        uint64_t dist_low = UINT64_MAX;

        /* Search downwards for the nearest prime ≤ k. */
        for (uint64_t p = (uint64_t)k; p >= 2; --p)
        {
            if (is_prime(p))
            {
                p_low = p;
                dist_low = k - p;
                break;
            }
        }

        /* Search upwards for the nearest prime ≥ k. */
        uint64_t p_high = 0;
        for (uint64_t p = (uint64_t)k; ; ++p)
        {
            if (is_prime(p))
            {
                p_high = p;
                break;
            }
        }

        uint64_t dist_high = p_high - k;
        uint64_t dist = (dist_low <= dist_high) ? dist_low : dist_high;
        dk = (seq_t)dist;

        /* Apply the recurrence a(k) = d(k)*a(k-1) + a(k-2). */
        cache[k] = dk * cache[k - 1] + cache[k - 2];
    }

    cached_size = new_size;
    return cache[n];
}
