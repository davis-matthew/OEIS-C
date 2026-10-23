/*
 * A185248  Expansion of 3F2( (1/2, 3/2, 5/2); (3, 5))(64 x).
 * https://oeis.org/A185248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   ensure_cache, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 16 * (2*n+3) * (2*n+1)^2 * (2*n)!^3 /
 *              (n!^4 * (n+2)! * (n+4)!)
 * Recurrence:
 *   a(0) = 1
 *   a(n) = a(n-1) * 8 * (2*n+3) * (2*n+1) * (2*n-1) /
 *          (n * (n+2) * (n+4)),  n >= 1
 *
 * This implementation computes the recurrence while canceling common
 * factors with the denominator to avoid overflow in 64‑bit builds.
 * It memoises previously computed values so that arbitrary call order
 * is supported.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* memoisation storage */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_max = 0;

    /* make sure the cache can hold values up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise a(0) only once */
    if (cached_max == 0) {
        cache[0] = (seq_t)1;
        cached_max = 0;
    }

    /* compute any missing values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* numerator = 8 * (2*i+3) * (2*i+1) * (2*i-1) */
        seq_t num  = (seq_t)8;
        num *= (seq_t)(2 * i + 3);
        num *= (seq_t)(2 * i + 1);
        num *= (seq_t)(2 * i - 1);

        /* denominator = i * (i+2) * (i+4) */
        seq_t den  = (seq_t)i;
        den *= (seq_t)(i + 2);
        den *= (seq_t)(i + 4);

        /* cancel common factors with prev */
        seq_t g = gcd(prev, den);
        prev /= g;
        den  /= g;

        /* cancel remaining common factors between num and den */
        g = gcd(num, den);
        num /= g;
        den  /= g;            /* den should now be 1 */

        cache[i] = prev * num;    /* final result */
        if (den != (seq_t)1)      /* safety check, should not occur */
            cache[i] /= den;
    }

    cached_max = n;
    return cache[n];
}
