/*
 * A030041  a(n+1) = Sum_{k=0..sqrt(n)} a(k) * a(n-k).
 * https://oeis.org/A030041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1
 *                a(n) = ∑_{k=0..⌊√(n-1)⌋} a(k) * a(n-1-k)   (n ≥ 1)
 * This implements the OEIS definition for A030041. */

#include <stdlib.h>

/* --- helper: integer square‑root for unsigned 64‑bit values --- */
static inline idx_t floor_sqrt_u64(idx_t v) {
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* use __int128 to avoid intermediate overflow of mid*mid */
        if ((__int128)mid * mid <= v) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;               /* high is the greatest square ≤ v */
}

/* static cache to enable memoised, out‑of‑order calls */
static seq_t *cache      = NULL;   /* array of computed values */
static size_t cache_cap  = 0;      /* allocated capacity (elements) */
static idx_t  cache_size = 0;      /* number of values we have filled (highest index+1) */

/* enlarge cache and compute missing terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* allocate / expand backing array */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure: exit (terminate) */ abort(); }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise very first term if needed */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* compute missing values */
    for (idx_t i = cache_size; i <= n; ++i) {
        idx_t kmax = floor_sqrt_u64(i - 1);   /* ⌊√(i‑1)⌋   */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= kmax; ++k) {
            sum += cache[k] * cache[(i - 1) - k];
        }
        cache[i] = sum;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
