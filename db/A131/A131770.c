/*
 * A131770  First differences (A131771) equal this sequence with terms
 *          repeated at positions: {m*(m+1)/2, m>=0}.
 * https://oeis.org/A131770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n - floor((sqrt(8*n+17)-3)/2)), a(0)=1
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root of an unsigned 64‑bit integer */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;      /* sqrt(2^64‑1) < 2^32 */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid == 0) {
            hi = mid - 1;
            continue;
        }
        if (mid > x / mid)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

/* static cache of computed values */
static seq_t *cache = NULL;
static size_t cache_size = 0;    /* current number of valid entries (>=1) */
static size_t cache_cap  = 0;    /* allocated capacity */

/* enlarge cache if needed */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap << 1 : 64;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                          /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = newcap;
}

/* sequence A131770 ------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise cache once */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            abort();                      /* unrecoverable allocation failure */
        cache_cap = 1;
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* if already computed, return immediately */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache to accommodate values up to n */
    ensure_cache_cap((size_t)n + 1);

    /* compute missing values in order */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {                    /* a(0) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }
        /* t(i) = floor((sqrt(8*i + 17) - 3)/2) */
        uint64_t radicand = 8ULL * (uint64_t)i + 17ULL;
        uint64_t sq = isqrt_u64(radicand);
        idx_t t = (sq >= 3 ? (sq - 3ULL) >> 1U : 0ULL);
        idx_t j = i - t;
        cache[i] = cache[i - 1] + cache[j];
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
