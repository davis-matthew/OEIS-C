/*
 * A068779  Least number k such that floor( k / digit reversal of k ) = n.
 * https://oeis.org/A068779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least k such that floor( k / rev(k) ) = n (rev(k) is the
 *            decimal digit reversal of k)
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   rev, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/rev.h"
#include "../utils/seq_realloc.h"

/* Cached values for a(1), a(2), ...  (index 0 unused) */
static seq_t *cache      = NULL;
static size_t   cache_cap = 0;          /* number of allocated slots   */
static idx_t    next_k   = 1;           /* next k to examine in scan   */
static idx_t    max_needed     = 0;     /* largest n requested so far   */
static idx_t    remaining_needed = 0;   /* how many 1…max_needed are still missing */

/* Ensure the cache has at least (idx+1) slots (since we use 1‑based indices). */
static void ensure_cache_cap(idx_t idx)
{
    if (idx + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 128;
    while (idx + 1 > new_cap)
        new_cap <<= 1;

    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    if (!cache)
        cache = new_cache;
    else
        cache = new_cache;

    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset 1, not used for n=0 */

    /* Make sure we can address a(n) and use it later. */
    ensure_cache_cap(n);

    /* Update the range of indices that must be known. */
    if (n > max_needed) {
        for (idx_t i = max_needed + 1; i <= n; ++i) {
            if (cache[i] == 0)
                ++remaining_needed;
        }
        max_needed = n;
    }

    /* Scan k values until all indices up to max_needed have been found. */
    while (remaining_needed > 0) {
        idx_t revk = rev(next_k);
        idx_t t = next_k / revk;   /* floor division, revk > 0 */

        if (t > 0) {
            ensure_cache_cap(t);
            if (cache[t] == 0) {    /* first time we see this n */
                cache[t] = (seq_t)next_k;
                if (t <= max_needed)
                    --remaining_needed;
            }
        }
        ++next_k;
    }

    return cache[n];
}
