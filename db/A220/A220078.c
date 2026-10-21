/*
 * A220078  a(n) = 5^(n!).
 * https://oeis.org/A220078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   ensure_cache, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/seq_pow.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < max_computed)
        return cache[n];

    /* Make sure the cache can hold terms up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute missing terms sequentially */
    for (idx_t k = max_computed; k <= n; ++k)
    {
        if (k == 0)
        {
            /* Base case: a(0) = 5 */
            cache[0] = (seq_t)5;
        }
        else
        {
            /* Recurrence: a(k) = a(k-1)^k */
            cache[k] = seq_pow(cache[k - 1], k);
        }
    }

    /* Update the number of terms that have been computed */
    max_computed = n + 1;
    return cache[n];
}
