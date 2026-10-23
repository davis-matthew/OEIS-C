/*
 * A037519  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,0,1.
 * https://oeis.org/A037519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037519: a(n) = floor((67/333) * 10^n)
   Recurrence (valid for n ≥ 5):
     a(1) = 2
     a(2) = 20
     a(3) = 201
     a(4) = 2012
     a(n) = 10 * a(n-1) + a(n-3) - 10 * a(n-4)   */

/* This function memoises computed terms so that calls in arbitrary
 * order are efficient.  It relies on the helper ensure_cache to grow
 * the backing array while preserving existing data. */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t   *cache     = NULL;    /* array of terms */
    static size_t  cache_cap  = 0;       /* allocated capacity */
    static size_t  cache_len  = 0;       /* number of terms stored   */

    if (n == 0) {
        return 0;                       /* undefined for offset 0 */
    }

    /* make sure the array is large enough for index n */
    if ((idx_t)cache_cap <= n) {
        ensure_cache((idx_t)(n + 1), &cache, &cache_cap);
    }

    /* initialise base values on first use */
    if (cache_len == 0) {
        if ((idx_t)cache_cap < 6) {      /* need indices 0..4 */
            ensure_cache(6, &cache, &cache_cap);
        }
        cache[0] = 0;                    /* unused but keeps indices tidy */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)201;
        cache[4] = (seq_t)2012;
        cache_len = 5;                   /* we have filled indices 0..4 */
    }

    /* compute any missing terms up to n */
    if (cache_len <= (size_t)n) {
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            cache[i] = 10 * cache[i - 1] + cache[i - 3] - 10 * cache[i - 4];
        }
        cache_len = (size_t)(n + 1);
    }

    return cache[n];
}
