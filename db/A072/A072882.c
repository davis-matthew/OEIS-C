/*
 * A072882  A nonlinear recurrence of order 3: a(1)=a(2)=a(3)=1;
 *          a(n)=(a(n-1)+a(n-2))^2/a(n-3).
 * https://oeis.org/A072882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (a(n-1)+a(n-2))^2 / a(n-3) (for n ≥ 4) Initial values:
 *            a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;          /* cache for computed values */
static size_t cache_cap = 0;         /* current allocated capacity */
static size_t cache_len = 0;         /* number of values already stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Make sure the cache can hold at least n values (1‑based index). */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the first three terms if they are not yet stored. */
    if (cache_len < 3) {
        cache[0] = (seq_t)1;       /* a(1) */
        cache[1] = (seq_t)1;       /* a(2) */
        cache[2] = (seq_t)1;       /* a(3) */
        cache_len = 3;
    }

    /* Compute terms up to a(n).  Use idx_t for loop variable, but cast
       to size_t when indexing the array. */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t a1 = cache[(size_t)(i - 1)];
        seq_t a2 = cache[(size_t)(i - 2)];
        seq_t a3 = cache[(size_t)(i - 3)];
        seq_t sum = a1 + a2;
        seq_t val = (sum * sum) / a3;
        cache[(size_t)i] = val;
    }

    cache_len = n;
    return cache[(size_t)(n - 1)];
}
