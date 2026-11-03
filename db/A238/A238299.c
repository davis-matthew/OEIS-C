/*
 * A238299  Second convolution of A107841.
 * https://oeis.org/A238299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+2)*(2*n-1)*a(n) = 4*(5*n^2-2)*a(n-1) - (n-2)*(2*n+1)*a(n-2)
 *            with initial values a(0)=1, a(1)=4. The function caches
 *            previously computed terms so that calls in any order are fast
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 992 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute a single term using the 2‑order recurrence, for n >= 2. */
static inline seq_t _compute_term(idx_t n, const seq_t *c)
{
#if defined(__cplusplus)
    /* C++ build – seq_t may be a big‑int class.  Use normal arithmetic. */
    seq_t ia    = (seq_t)n;
    seq_t t1    = 4 * ((5 * ia * ia - 2) * c[n-1]);
    seq_t t2    = ((n-2) * (2 * ia + 1)) * c[n-2];
    seq_t numer = t1 - t2;
    seq_t denom = (ia + 2) * (2 * ia - 1);
    return numer / denom;
#else
    /* C build – use unsigned __int128 for intermediate values.  Only
       values that fit into seq_t will be requested. */
    unsigned __int128 ii = n;
    unsigned __int128 factor1 = 4 * (5 * ii * ii - 2);
    unsigned __int128 term1   = factor1 * (unsigned __int128)c[n-1];

    unsigned __int128 factor2 = (ii - 2) * (2 * ii + 1);
    unsigned __int128 term2   = factor2 * (unsigned __int128)c[n-2];

    unsigned __int128 numer   = term1 - term2;
    unsigned __int128 denom   = (ii + 2) * (2 * ii - 1);

    return (seq_t)(numer / denom);
#endif
}

/* Cache for previously computed terms.  The array holds a(0) through
   a(cache_len-1).  It is grown on demand. */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of values currently stored */
static size_t cache_cap = 0;   /* allocated capacity in elements */

/* Ensure the cache can store terms at indices up to 'need' (inclusive). */
static void _grow_cache(idx_t need)
{
    if ((size_t)(need + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while ((size_t)(need + 1) > new_cap)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap != 0) {
            mem_free(cache);
            cache = NULL;
            cache_cap = 0;
            return;
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n < cache_len)
        return cache[n];

    /* On first use, initialise the cache with the two known terms. */
    if (cache_len == 0) {
        _grow_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* Ensure enough space for the requested index. */
    if (n >= cache_len)
        _grow_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            /* already initialised */
            continue;
        }
        cache[i] = _compute_term(i, cache);
    }
    cache_len = n + 1;
    return cache[n];
}
