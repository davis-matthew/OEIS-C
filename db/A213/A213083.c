/*
 * A213083  Each square n^2 appears n^2 number of times.
 * https://oeis.org/A213083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k^2 where k is the smallest integer with k(k+1)(2k+1)/6
 *            >= n. Equivalently, let m = floor((3n)^(1/3)). If n >
 *            m(m+1)(2m+1)/6 then k = m+1, else k = m
 * Range:     u8 n <= 1240, u16 n <= 10416, u32 n <= 10416, u64 n <= 10416,
 *            u128 n <= 10416, big any n
 * Validated: standard, pass, 84 terms through n = 9329 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t floor_cube_root_3n(idx_t n)
{
    /* Compute floor( (3*n)^(1/3) ) using 128‑bit arithmetic. */
    __uint128_t x = (__uint128_t)3 * n;
    idx_t lo = 0;
    idx_t hi = 1;

    /* Exponentially increase hi until hi^3 > x. */
    while ((__uint128_t)hi * hi * hi <= x) {
        hi <<= 1;
    }
    lo = hi >> 1;
    /* Binary search between lo and hi. */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __uint128_t mid3 = (__uint128_t)mid * mid * mid;
        if (mid3 <= x) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n >= 1; offset 1. */
    idx_t m = floor_cube_root_3n(n);
    seq_t mm = (seq_t)m;
    seq_t S = mm * (mm + 1) * (2 * mm + 1) / 6;   /* S = m(m+1)(2m+1)/6 */
    if ((seq_t)n > S) {
        ++m;
    }
    seq_t k = (seq_t)m;
    return k * k;   /* k^2 */
}
