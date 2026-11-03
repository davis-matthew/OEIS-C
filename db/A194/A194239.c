/*
 * A194239  a(n) = [Sum_{k=1..n} (r*k)], where [ ]=floor, ( )=fractional part,
 *          and r=(1+sqrt(2))/2.
 * https://oeis.org/A194239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( r * n(n+1)/2 ) - Σ_{k=1..n} floor( r*k ), where r
 *            = (1+sqrt(2))/2 and floor is the integer part. This is
 *            equivalent to a(n) = floor( Σ_{k=1..n} frac(r*k) )
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 73, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9377 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Convert the index to seq_t */
    const seq_t N = (seq_t)n;
    const seq_t ONE = (seq_t)1;

    /* n(n+1)/2 */
    seq_t M = (N * (N + ONE)) >> 1;

    /* floor(r * M) = floor( (M + floor( sqrt(2*M^2) )) / 2 ) */
    seq_t M_sq = M * M;
    seq_t two_M_sq = M_sq + M_sq;            /* 2 * M^2 */
    seq_t sqrt_two_M_sq = floor_sqrt(two_M_sq);
    seq_t floor_r_M = (M + sqrt_two_M_sq) >> 1;

    /* Σ_{k=1..n} floor(r*k) */
    seq_t sum_floor = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        const seq_t K = (seq_t)k;
        seq_t K_sq = K * K;
        seq_t two_K_sq = K_sq + K_sq;       /* 2 * k^2 */
        seq_t sqrt_two_K_sq = floor_sqrt(two_K_sq);
        seq_t floor_r_K = (K + sqrt_two_K_sq) >> 1;
        sum_floor += floor_r_K;
    }

    /* a(n) = floor(r*M) - Σ floor(r*k) */
    return floor_r_M - sum_floor;
}
