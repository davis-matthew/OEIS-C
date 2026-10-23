/*
 * A036571  Binary packing of Connell sequence (shifted once right).
 * https://oeis.org/A036571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(n)=a(n-1)+2^((2*n - floor((1+sqrt(8*n-7))/2)) - 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"


/* Integer square root for unsigned 64‑bit integers (no floating point). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1ULL << 32;          /* sqrt(ULLONG_MAX) < 2^32 */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) {                      /* avoid div by zero */
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {                /* mid*mid <= n  */
            ans = mid;
            low = mid + 1;
        } else {                             /* mid*mid > n   */
            high = mid - 1;
        }
    }
    return ans;
}

/* Cached values for non‑sequential access */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t cache_len = 0;       /* highest index computed + 1 */

/* Computes a(n) with memoisation.  Works for any order of calls. */
seq_t a(idx_t n)
{
    /* Initialise cache if first call. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;           /* a(0) */
        cache_len = 1;
    }

    /* Grow the cache array if necessary. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* k = floor((1 + sqrt(8*i - 7)) / 2) */
        idx_t val = 8 * i - 7;
        idx_t s = isqrt_u64(val);
        idx_t k = (1 + s) >> 1;

        /* exponent e = 2*i - k - 1 */
        idx_t e = 2 * i - k - 1;

        cache[i] = cache[i - 1] + pow2(e);
    }

    cache_len = n + 1;
    return cache[n];
}
