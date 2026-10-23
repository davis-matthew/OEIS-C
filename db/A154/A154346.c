/*
 * A154346  a(n) = 12*a(n-1) - 28*a(n-2) for n > 1, with a(0)=1, a(1)=12.
 * https://oeis.org/A154346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 28*a(n-2) a(0) = 1, a(1) = 12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t curmax = 0;

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    /* ensure storage for indices 0..n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise base values if necessary */
    if (curmax < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        curmax   = 1;
    }

    /* extend the table up to n */
    for (idx_t i = curmax + 1; i <= n; ++i) {
        cache[i] = (seq_t)12 * cache[i - 1] - (seq_t)28 * cache[i - 2];
    }
    curmax = n;

    return cache[n];
}
