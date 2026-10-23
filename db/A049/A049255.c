/*
 * A049255  Smallest nonnegative value taken on by 16x^2 - ny^2 for an
 *          infinite number of integer pairs (x, y).
 * https://oeis.org/A049255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest nonnegative k such that 16*x^2 - n*y^2 = k has
 *            infinitely many integer solutions. This is equivalent to finding
 *            the minimal k ≥ 0 for which the Pell-type equation X^2 - n*Y^2 =
 *            k has a solution with X ≡ 0 (mod 4); then infinite solutions
 *            exist
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 128‑bit unsigned integers */
static inline __uint128_t isqrt_uint128(__uint128_t v) {
    __uint128_t lo = 0, hi = ((__uint128_t)1 << 64) - 1;   /* hi = 2^64 - 1 */
    while (lo <= hi) {
        __uint128_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;   /* floor(sqrt(v)) */
}

/* 64‑bit integer square‑root (floor) – used only for perfect‑square test */
static inline idx_t isqrt_u64(idx_t v) {
    idx_t lo = 0, hi = (idx_t)-1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Main OEIS sequence function */
seq_t a(idx_t n) {
    /* 0 for perfect squares – obtain integer solution X=sqrt(n)*Y with both X and Y integer */
    idx_t r = isqrt_u64(n);
    if (r * r == n) {
        return (seq_t)0;
    }

    /* Search for minimal k>0 */
    const idx_t Y_MAX = 20000;   /* generous bound – 20 000 iterations are trivial */
    __uint128_t min_k = (~(__uint128_t)0);   /* initialise to maximum 128‑bit value */

    for (idx_t y = 1; y <= Y_MAX; ++y) {
        /* Compute T = n*y^2 and its integer square root */
        __uint128_t temp = (__uint128_t)n * y * y;
        __uint128_t sqrt_floor = isqrt_uint128(temp);

        /* Find the least multiple of 4 ≥ sqrt_floor */
        __uint128_t X = ((sqrt_floor + 3) / 4) * 4;
        /* Ensure X^2 ≥ T (rarely needed when sqrt_floor < sqrt(T)) */
        while (X * X < temp) {
            X += 4;
        }

        __uint128_t k = X * X - temp;
        if (k < min_k) {
            min_k = k;
            if (min_k == ( __uint128_t)1) {
                break;   /* cannot get smaller than 1 for non‑square n */
            }
        }
    }

    return (seq_t)min_k;
}
