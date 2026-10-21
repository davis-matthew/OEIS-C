/*
 * A063014  Number of solutions to n^2 = b^2 + c^2 (with c >= b >= 0).
 * https://oeis.org/A063014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (b,c) with 0 ≤ b ≤ c and n² =
 *            b² + c². It counts the lattice points on the circle of radius n
 *            with non‑decreasing coordinates
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer square root for 64‑bit values, implemented without extra
   helpers to avoid overflow. */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;          /* sqrt(2⁶⁴‑1) < 2³² */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Cache for previously computed a(n). */
static seq_t *a_cache = NULL;
static idx_t  cache_len = 0;

/* Count representations of n² = b² + c² with 0 ≤ b ≤ c.  For n==0
   the only solution is (0,0). */
static seq_t count_reps(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* n² fits easily in 64 bits for the required limits. */
    uint64_t n2   = (uint64_t)n * (uint64_t)n;
    seq_t    count = (seq_t)1;              /* (b,c) = (0,n) */

    /* b ranges up to ⌊n/√2⌋.  The division by √2 is done once per n. */
    idx_t maxb = (idx_t)((double)n / 1.4142135623730951);

    for (idx_t b = 1; b <= maxb; ++b) {
        uint64_t bsq = (uint64_t)b * (uint64_t)b;
        uint64_t rest = n2 - bsq;
        uint64_t c = isqrt_uint64(rest);
        if (c * c == rest && c >= b)
            ++count;
    }
    return count;
}

/* Ensure the cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    idx_t needed = n + 1;
    idx_t cap = cache_len ? cache_len : 64;
    while (cap < needed) cap <<= 1;

    if (cap > cache_len) {
        a_cache = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        a_cache[i] = count_reps(i);
    }
    cache_len = needed;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
