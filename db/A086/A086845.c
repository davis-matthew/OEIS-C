/*
 * A086845  a(1) = 0, a(n) = a(floor(n/2)) + 2*a(ceiling(n/2)) + floor(n/2).
 * https://oeis.org/A086845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(n) = a(floor(n/2)) + 2*a(ceil(n/2)) + floor(n/2)
 * Range:     u8 n <= 33, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoized solution that is correct for all supported seq_t types
 * (uint8_t … uint64_t, unsigned __int128, or arbitrary‑precision). */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;           /* cache[i] holds a(i) for i ≥ 1 */
static size_t  cache_cap = 0;         /* allocated size (index 0 is unused) */
static idx_t   cache_max = 0;         /* largest n that has been computed */

/* Ensure that the cache can index up to n (inclusive).  Grow it if needed
 * and compute all missing values in order so that the recurrence can be
 * evaluated using already cached results. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;           /* already computed */

    /* Determine required capacity (index 0..n). */
    size_t needed = (size_t)(n + 2);       /* +1 for index 0, +1 for safety around overflow */

    /* Grow the cache if necessary. */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* out of memory – test harness will handle */

        cache = new_cache;
        cache_cap = new_cap;

        /* Initialise new slots to zero (indices 0..new_cap-1). */
        for (size_t i = cache_max + 1; i < cache_cap; ++i) cache[i] = (seq_t)0;
    }

    /* Initialise base case if not already done. */
    if (cache_max < 1) {
        cache[1] = (seq_t)0;
    }

    /* Forward compute new terms. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        idx_t f = k >> 1;              /* floor(k/2) */
        idx_t c = k - f;               /* ceil(k/2)  */
        cache[k] = cache[f] + (seq_t)2 * cache[c] + (seq_t)f;
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, but return 0 safely */
    ensure_cache(n);
    return cache[n];
}
