/*
 * A127321  First 4-dimensional hyper-tetrahedral coordinate; repeat m
 *          C(m+3,3) times; 4-D analog of A056556.
 * https://oeis.org/A127321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal m such that C(m+4,4) > n (where C(a,b) = binomial
 *            coefficient)
 * Range:     u8 n <= 10625, u16 n <= 10625, u32 n <= 10625, u64 n <= 10625,
 *            u128 n <= 10625, big any n
 * Validated: standard, pass, 121 terms through n = 9342 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute C(m+4,4) using 128‑bit arithmetic so it never overflows. */
static inline __uint128_t binom_m4_4(idx_t m)
{
    __uint128_t v = (__uint128_t)(m + 4);
    v = v * (m + 3) * (m + 2) * (m + 1) / 24;
    return v;
}

seq_t a(idx_t n)
{
    /* Find the smallest m with C(m+4,4) > n. */
    idx_t low = 0;
    idx_t high = 1;

    /* Double high until it exceeds n. */
    while (true) {
        __uint128_t val = binom_m4_4(high);
        if (val > ( __uint128_t )n)
            break;
        /* Avoid overflow of idx_t. */
        if (high > (UINT64_MAX >> 1)) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;   /* high = high * 2 */
    }

    /* Binary search on [low, high]. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t val = binom_m4_4(mid);
        if (val > ( __uint128_t )n)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
