/*
 * A020371  Numbers k such that the continued fraction for sqrt(k) has period
 *          32.
 * https://oeis.org/A020371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers k such that the continued fraction of sqrt(k)
   has period 32.  The function searches k in increasing order, checks
   the period of the continued fraction using the standard algorithm,
   and memoises the results so that each term is computed only once. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a 64‑bit unsigned integer. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t low  = 0;
    uint64_t high = 1ULL << 32;          /* sqrt(UINT64_MAX) < 2^32 */
    uint64_t mid, r = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

/* Return the period of the continued fraction expansion of sqrt(D),
   or -1 if D is a perfect square. */
static int period_for_D(uint64_t D)
{
    uint64_t a0 = isqrt_u64(D);
    if (a0 * a0 == D) return -1;          /* perfect square */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    int period = 0;

    for (;;) {
        /* m = d*a - m */
        m = d * a - m;

        /* d = (D - m*m) / d */
        d = (D - m * m) / d;

        /* a = (a0 + m) / d */
        a = (a0 + m) / d;

        ++period;

        /* period ends when a == 2*a0 */
        if (a == 2 * a0) break;
    }
    return period;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Indices below the offset are undefined; return 0. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_count = 0;
    static idx_t cache_cap   = 0;
    static uint64_t next_k   = 2;          /* start searching from 2 */

    /* Grow the cache to contain at least n elements. */
    while (cache_count < n) {
        /* Grow allocation size if needed. */
        if (cache_cap == 0 || cache_count >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next integer k whose continued‑fraction period is 32. */
        uint64_t k = next_k;
        while (true) {
            if (period_for_D(k) == 32) {
                cache[cache_count++] = (seq_t)k;
                break;
            }
            ++k;
        }
        next_k = k + 1;          /* next search starts after the found value */
    }

    return cache[n - 1];
}
