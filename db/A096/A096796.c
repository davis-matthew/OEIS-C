/*
 * A096796  a(n) = n for n <= 2; for n > 2, a(n) = 2a(n-1) - a(n - floor( 1/2
 *          + sqrt(2n) )).
 * https://oeis.org/A096796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for n ≤ 2; a(n) = 2*a(n-1) – a(n – round( sqrt(2n) ))
 *            for n > 2 where round(x) = ⌊x + ½⌋ (nearest integer)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline idx_t isqrt_u64(idx_t x) {
    /* Integer floor of sqrt(x); works for all x ≤ UINT64_MAX.
       A simple binary search is sufficient for the sizes used here. */
    idx_t lo = 0, hi = (idx_t)1 << ((sizeof(idx_t)*8)/2);
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    /* Auxiliary static cache to allow arbitrary call order. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache has enough space. */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Initialize new entries if we are extending. */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i <= 2) {
                cache[i] = (seq_t)i;
            } else {
                /* compute a(i) using the recurrence */
                seq_t two_prev = cache[i-1] + cache[i-1];
                /* compute round( sqrt(2*i) ) */
                idx_t t = isqrt_u64(2*i);
                idx_t diff = (2*i) - t*t;
                idx_t k = (diff >= t + 1) ? (t + 1) : t;
                idx_t j = i - k;
                cache[i] = two_prev - cache[j];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
