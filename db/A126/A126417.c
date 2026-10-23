/*
 * A126417  Number of base 30 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑30 n‑digit sequences whose adjacent digits
 *            differ by at most 2. a(0)=1, a(1)=30, and for n≥2 the DP
 *            recurrence below counts all valid sequences with first digit
 *            allowed to be 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;          /* a[0..computed_len] */
static size_t cache_cap = 0;
static idx_t computed_len = 0;       /* largest n whose value is cached */
static seq_t dp_prev[30];            /* DP for the previous length */
static bool dp_initialized = false;

/* Return a(n) for the OEIS sequence A126417. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    /* Ensure cache can hold at least a[1] in case we need to initialise. */
    if (!cache || cache_cap < 2)
        ensure_cache(2, &cache, &cache_cap);

    /* Initialise dp for length 1 only once. */
    if (!dp_initialized) {
        for (int d = 0; d < 30; ++d)
            dp_prev[d] = (seq_t)1;        /* one way to choose a single digit */

        seq_t sum = 0;
        for (int d = 0; d < 30; ++d)
            sum += dp_prev[d];
        cache[1] = sum;                   /* a(1) = 30 */
        computed_len = 1;
        dp_initialized = true;
    }

    /* If the requested value is already cached, return it. */
    if (n <= computed_len)
        return cache[n];

    /* Grow the cache to accommodate a[n]. */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Compute DP from (computed_len + 1) up to n. */
    for (idx_t len = computed_len + 1; len <= n; ++len) {
        seq_t dp_next[30];

        /* Transition: add all neighbours within distance 2. */
        for (int d2 = 0; d2 < 30; ++d2) {
            seq_t s = 0;
            if (d2 >= 2) s += dp_prev[d2 - 2];
            if (d2 >= 1) s += dp_prev[d2 - 1];
            s += dp_prev[d2];
            if (d2 + 1 < 30) s += dp_prev[d2 + 1];
            if (d2 + 2 < 30) s += dp_prev[d2 + 2];
            dp_next[d2] = s;
        }

        /* Sum all ending digits to get a[len]. */
        seq_t sum = 0;
        for (int d = 0; d < 30; ++d)
            sum += dp_next[d];
        cache[len] = sum;

        /* Prepare for the next length. */
        for (int d = 0; d < 30; ++d)
            dp_prev[d] = dp_next[d];
    }

    computed_len = n;
    return cache[n];
}
