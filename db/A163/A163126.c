/*
 * A163126  a(1)=1. a(n) = the number of integers k, 1 <= k <= n-1, where a(k)
 *          is coprime to n-k.
 * https://oeis.org/A163126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 333, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163126: a(1)=1, a(n)=# {1 <= k < n | gcd(a(k), n-k)=1}. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* Ensure the cache is large enough to index up to 'need'. */
static void ensure_cache_size(idx_t need)
{
    size_t required = (size_t)(need + 1);
    if (cache_cap >= required)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!new_cache)
        abort();

    cache = new_cache;
    cache_cap = required;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset starts at 1, guard against 0 */

    /* Make sure we can store values up to n. */
    ensure_cache_size(n);

    /* Compute any missing values up to n. */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        if (m == 1) {                 /* base case */
            cache[1] = (seq_t)1;
            continue;
        }

        seq_t count = 0;
        for (idx_t k = 1; k < m; ++k) {
            if (gcd(cache[k], (seq_t)(m - k)) == (seq_t)1)
                ++count;
        }
        cache[m] = count;
    }

    cache_max = n;
    return cache[n];
}
