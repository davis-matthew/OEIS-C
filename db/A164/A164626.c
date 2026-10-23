/*
 * A164626  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            13*a(n-1)+13*a(n-2)+13*a(n-3)+13*a(n-4)+13*a(n-5)+13*a(n-6)-91*a(n-7)
 *            for n ≥ 8; initial terms: a(0)=1, a(1)=15, a(2)=210, a(3)=2940,
 *            a(4)=41160, a(5)=576240, a(6)=8067360, a(7)=112942935
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* If n is within the initial range, return the hard‑coded value. */
    if (n <= 7) {
        static const seq_t init[] = {
            1ULL,      15ULL,       210ULL,      2940ULL,
            41160ULL,  576240ULL,   8067360ULL, 112942935ULL
        };
        return init[n];
    }

    /* Cache for values up to the maximum n requested so far. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cached_upto = 0; /* largest index already computed */

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the first eight values if this is the first call. */
    if (cached_upto < 7) {
        cache[0] =  1ULL;
        cache[1] =  15ULL;
        cache[2] = 210ULL;
        cache[3] = 2940ULL;
        cache[4] = 41160ULL;
        cache[5] = 576240ULL;
        cache[6] = 8067360ULL;
        cache[7] = 112942935ULL;
        cached_upto = 7;
    }

    /* Compute missing terms up to n using the linear recurrence. */
    for (size_t i = cached_upto + 1; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        sum += 13 * cache[i - 1];
        sum += 13 * cache[i - 2];
        sum += 13 * cache[i - 3];
        sum += 13 * cache[i - 4];
        sum += 13 * cache[i - 5];
        sum += 13 * cache[i - 6];
        sum -= 91 * cache[i - 7];
        cache[i] = sum;
    }

    cached_upto = (size_t)n;
    return cache[(size_t)n];
}
