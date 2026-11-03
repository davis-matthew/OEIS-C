/*
 * A184106  n+floor(6*sqrt(n)); complement of A184107.
 * https://oeis.org/A184106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(6 * sqrt(n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
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

static inline seq_t floor_sqrt_36_n(idx_t n)
{
    /* For small seq_t types use 128‑bit arithmetic to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t nn = (__uint128_t)n * 36ULL;
        __uint128_t lo = 0, hi = (__uint128_t)1 << 64; /* hi is surely ≥ sqrt(36*n) for n<2⁶⁴ */
        while (lo < hi) {
            __uint128_t mid = (lo + hi + 1) >> 1;
            __uint128_t sq  = mid * mid;
            if (sq <= nn)
                lo = mid;
            else
                hi = mid - 1;
        }
        return (seq_t)lo;
    } else {
        seq_t temp = (seq_t)36 * (seq_t)n;
        return floor_sqrt(temp);  /* utility for arbitrary‑precision types */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behavior below offset is irrelevant */
    seq_t N = (seq_t)n;
    seq_t s = floor_sqrt_36_n(n);
    return N + s;
}
