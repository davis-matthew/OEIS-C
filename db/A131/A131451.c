/*
 * A131451  Product of the nonzero digital products of all the numbers 1 to n
 *          (a 'total digital-product factorial' in base 10).
 * https://oeis.org/A131451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{k=1}^{n} dp_{10}(k), where dp_{10}(k) is the product
 *            of * the non‑zero decimal digits of k. (OEIS A131451)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *a_cache = NULL;
static size_t  a_cache_cap = 0;
static idx_t   a_cache_last = 0;   /* last index that has been computed */

seq_t a(idx_t n)
{
    if (n == 0)                     /* not part of the official sequence but safe */
        return (seq_t)1;

    /* make sure the cache can hold indices up to n */
    ensure_cache(n, &a_cache, &a_cache_cap);

    /* initialise the base case */
    if (a_cache_last == 0)
        a_cache[0] = (seq_t)1;

    /* compute missing values up to n (the loop always starts at 1) */
    for (idx_t i = a_cache_last + 1; i <= n; ++i) {
        seq_t dp = (seq_t)1;            /* non‑zero digit product of i */
        idx_t v  = i;
        while (v != 0) {
            idx_t d = v % 10;
            if (d != 0)
                dp = dp * (seq_t)d;
            v /= 10;
        }
        a_cache[i] = a_cache[i - 1] * dp;
    }

    a_cache_last = n;
    return a_cache[n];
}
