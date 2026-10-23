/*
 * A129557  Numbers k > 0 such that k^2 is a centered pentagonal number
 *          (A005891).
 * https://oeis.org/A129557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n >= 5, a(n) = 38 * a(n-2) - a(n-4); Initial values: a(1)=1,
 *            a(2)=4, a(3)=34, a(4)=151
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;   /* array of cached values, indices start at 1 */
static size_t cache_cap = 0;  /* maximum index that cache can hold (inclusive) */
static idx_t computed_max = 0;/* largest index that has been filled */

/* Return the n-th term of sequence A129557 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* sequence is defined for n >= 1 */

    /* Make sure the cache can hold the requested index */
    ensure_cache(n, &cache, &cache_cap);

    /* Base values (indices 1..4) – always stored */
    static const seq_t base[5] = {0, 1, 4, 34, 151};
    for (idx_t i = 1; i <= 4 && i <= n; ++i) cache[i] = base[i];

    /* If only the base values are requested, return immediately */
    if (n <= 4) {
        computed_max = n;
        return cache[n];
    }

    /* Compute all missing values up to n using the recurrence */
    idx_t start = (computed_max < 4 ? 5 : computed_max + 1);
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)38 * cache[i - 2] - cache[i - 4];
    }
    computed_max = n;
    return cache[n];
}
