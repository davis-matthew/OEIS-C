/*
 * A194682  Number of k in [1,n] for which <n*r>+<k*r> > 1, where < > =
 *          fractional part, and r=3-sqrt(2); row sums of A164681.
 * https://oeis.org/A194682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k∈[1,n] : ⟨n·r⟩ + ⟨k·r⟩ > 1 }| where r = 3 – √2.
 *            Equivalently a(n) = |{ k∈[1,n] : ⟨-n·√2⟩ > ⟨k·√2⟩ }|, which can
 *            be tested by the integer inequality floor((k+n)·√2) ≤
 *            floor(k·√2) + floor(n·√2)
 * Range:     u8 n <= 267, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute floor(√x) for 0 ≤ x ≤ 2^63 – 1 using binary search.
   This function never uses floating point operations. */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = (x < 0xFFFFFFFFULL) ? 0x1FFFFULL : 0x8000ULL; /* safe high */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            if ((mid + 1) * (mid + 1) > x) return mid;
            lo = mid + 1;
        } else {
            if (mid == 0) return 0;
            hi = mid - 1;
        }
    }
    return 0;
}

/* Maximum n for which the user is expected to call a(n). */
#define MAX_N 10000U

/* Static cache of the sequence values.  The array is allocated on first use. */
static seq_t *a_cache = NULL;

/* Helper arrays for floor(k·√2).  They are allocated once. */
static uint64_t *t_cache = NULL;
static size_t t_cache_len = 0; /* length of t_cache  (max index + 1) */

/* Allocate and fill t_cache up to index 'limit' (inclusive). */
static void build_t_cache(size_t limit)
{
    if (t_cache_len > limit) return;
    size_t new_len = limit + 1;
    if (t_cache == NULL) {
        t_cache = (uint64_t *)malloc(new_len * sizeof(uint64_t));
    } else {
        t_cache = (uint64_t *)realloc(t_cache, new_len * sizeof(uint64_t));
    }
    for (size_t m = t_cache_len; m <= limit; ++m) {
        /* floor(m·√2) = floor(√(2·m²)) */
        uint64_t val = 2ULL * (uint64_t)m * (uint64_t)m;
        t_cache[m] = isqrt_uint64(val);
    }
    t_cache_len = limit;
}

/* Build the full a(n) cache up to MAX_N. */
static void build_a_cache(void)
{
    if (a_cache != NULL) return;
    a_cache = (seq_t *)malloc((MAX_N + 1) * sizeof(seq_t));
    /* We need t[k] for k up to 2*MAX_N */
    build_t_cache(2 * MAX_N);
    for (idx_t n = 1; n <= MAX_N; ++n) {
        uint64_t tn = t_cache[n];
        uint64_t count = 0;
        for (idx_t k = 1; k <= n; ++k) {
            /* floor((k+n)·√2) <= floor(k·√2) + floor(n·√2) */
            if (t_cache[k + n] <= t_cache[k] + tn) {
                ++count;
            }
        }
        a_cache[n] = (seq_t)count;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined for 0, return 0 */
    if (n > (idx_t)MAX_N) {
        /* For n larger than the pre‑computed range we recompute on the fly.
           This is slower but still holds the exact definition.  */
        /* Allocate temporary arrays with size 2*n */
        size_t limit = 2 * (size_t)n;
        uint64_t *t = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
        for (size_t m = 0; m <= limit; ++m) {
            uint64_t val = 2ULL * (uint64_t)m * (uint64_t)m;
            t[m] = isqrt_uint64(val);
        }
        uint64_t count = 0;
        uint64_t tn = t[n];
        for (size_t k = 1; k <= (size_t)n; ++k) {
            if (t[k + n] <= t[k] + tn) ++count;
        }
        free(t);
        return (seq_t)count;
    }
    build_a_cache();
    return a_cache[n];
}
