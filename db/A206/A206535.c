/*
 * A206535  Number of (n+1) X 2 0..2 arrays with the number of clockwise edge
 *          increases in 2 X 2 subblocks nondecreasing, and counterclockwise
 *          edge increases nonincreasing, rightwards and downwards.
 * https://oeis.org/A206535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A206535: Number of (n+1)×2 0..2 arrays with the number of
 * clockwise edge increases in 2×2 subblocks nondecreasing, and
 * counter‑clockwise edge increases nonincreasing, rightwards and
 * downwards.
 *
 * Empirical recurrence (valid for n > 8):
 *   a(n) = 7*a(n-1) - 9*a(n-2) - 15*a(n-3) + 18*a(n-4)
 *
 * Initial terms (offset 1):
 *   a(1)=81,  a(2)=399,  a(3)=1620,  a(4)=6462,
 *   a(5)=26481, a(6)=110769, a(7)=469617, a(8)=2009553
 */

#include <stddef.h>          /* size_t */
#include "../utils/seq_realloc.h"   /* seq_realloc */

/* static cache for already computed values; cache[0] is unused     */
static seq_t *cache      = NULL;   /* array of a(n) for 1..cache_len   */
static size_t cache_cap  = 0;      /* capacity of cache (indices 0..cap-1) */
static idx_t  cache_len  = 0;      /* highest index already computed  */

/* initialise six known values; index 0 is unused and set to 0 */
static const seq_t init_vals[9] = {
    (seq_t)0,
    (seq_t)81,
    (seq_t)399,
    (seq_t)1620,
    (seq_t)6462,
    (seq_t)26481,
    (seq_t)110769,
    (seq_t)469617,
    (seq_t)2009553
};

/* ensure that the cache can hold indices up to n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        cache = seq_realloc(NULL, 0, needed);
        cache_cap = needed;
    } else if (needed > cache_cap) {
        cache = seq_realloc(cache, cache_cap, needed);
        cache_cap = needed;
    }
}

/* compute a(n) for any n >= 1; any order of calls is supported */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n==0 but safe */

    ensure_cache_capacity(n);

    /* Return a value that is already cached */
    if (n <= cache_len) return cache[n];

    /* Compute missing terms up to n (iteratively) */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init_vals[i];    /* copy constant initial value */
        } else {
            /* recurrence: a(i) = 7*a(i-1) - 9*a(i-2) - 15*a(i-3) + 18*a(i-4) */
            seq_t term = cache[i - 1];
            term *= (seq_t)7;
            term -= cache[i - 2] * (seq_t)9;
            term -= cache[i - 3] * (seq_t)15;
            term += cache[i - 4] * (seq_t)18;
            cache[i] = term;
        }
    }

    cache_len = n;
    return cache[n];
}
