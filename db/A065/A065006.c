/*
 * A065006  Integers for which the periodic part of the continued fraction for
 *          the square root of n begins with 3.
 * https://oeis.org/A065006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth integer for which the first term of the
 * periodic part of the continued fraction of sqrt(n) equals 3.
 * 
 * The implementation computes values in order, caching previously
 * found terms.  The property is checked by a direct continued fraction
 * expansion of sqrt(n) using the standard recurrence:
 *
 *    m_{k+1} = d_k * a_k - m_k
 *    d_{k+1} = (n - m_{k+1}^2) / d_k
 *    a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})
 *
 * The period starts when a_k == 2*a0; the first a_k after a0 is the
 * first term of the period.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*<! Helper to compute integer square root of an unsigned 64‑bit integer. */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t r = (uint64_t) sqrt((long double) n);
    while (r * r > n) --r;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/*<! Test whether the first periodic coefficient of sqrt(n) equals 3. */
static bool first_period_is_three(uint64_t n) {
    uint64_t a0 = isqrt_u64(n);
    if (a0 * a0 == n) return false;           /* perfect square */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    /* Compute first a after a0 */
    m = d * a - m;
    unsigned __int128 tmp = (unsigned __int128) n - (unsigned __int128) m * m;
    d = (uint64_t) (tmp / d);
    a = (a0 + m) / d;
    if (a == 3) return true;

    /* Continue until the period repeats (a == 2*a0) */
    while (a != 2 * a0) {
        m = d * a - m;
        tmp = (unsigned __int128) n - (unsigned __int128) m * m;
        d = (uint64_t) (tmp / d);
        a = (a0 + m) / d;
    }
    return false;
}

/*<! Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_capacity = 0;
static uint64_t next_candidate = 2;

/*<! Allocate or grow the cache as needed. */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity == 0 ? 16 : cache_capacity;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *) realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1); /* abort on allocation failure */
    cache = new_arr;
    cache_capacity = new_cap;
}

/*<! Append a value to the cache. */
static void append_value(seq_t v) {
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = v;
}

/*<! Main sequence function */
seq_t a(idx_t n) {
    /* Compute terms until we have at least n terms. */
    while (cache_len < n) {
        uint64_t cand = next_candidate++;
        if (first_period_is_three(cand)) {
            append_value((seq_t) cand);
        }
    }
    return cache[n - 1];
}
