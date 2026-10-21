/*
 * A099477  Numbers having no divisors d such that also d+2 is a divisor.
 * https://oeis.org/A099477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A099477: numbers whose divisors contain no two integers differing by 2.
   The sequence is generated on demand by searching consecutive integers and
   caching the results.  The cached values are reused for later calls. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
# include <new>
#endif

/* Integer square root (floor) for uint64_t */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;  /* hi^2 > max uint64_t */
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Test whether n has two divisors that differ by 2 */
static bool has_bad_pair(uint64_t n)
{
    uint64_t r = isqrt_uint64(n);
    for (uint64_t d = 1; d <= r; ++d) {
        if (n % d == 0) {
            if (n % (d + 2) == 0)
                return true;
        }
    }
    return false;
}

/* ---------------------  cache machinery  --------------------- */

static seq_t *cache = NULL;        /* 1‑indexed array of found terms */
static idx_t cache_len = 0;        /* number of terms stored */
static idx_t cache_cap = 0;        /* allocated capacity */
static uint64_t last_checked = 0;   /* last examined integer */

/* Ensure the cache can hold at least 'need' elements (need ≥ 1) */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t newcap = cache_cap ? cache_cap * 2 : 16;
    if (newcap < need)
        newcap = need;

#if defined(__cplusplus)
    /* C++: use new/delete to preserve copy‑constructors */
    seq_t *tmp = new seq_t[newcap];
    for (idx_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    delete[] cache;
    cache = tmp;
    cache_cap = newcap;
#else
    /* C: use realloc (single POD types are safe) */
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
#endif
}

/* ---------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; undefined for n==0 – return 0 */
        return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_checked;                      /* next natural number */
        if (!has_bad_pair(last_checked)) {
            ensure_cache(cache_len + 1);
            /* Store the term; implicit conversion from uint64_t to seq_t is fine */
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
