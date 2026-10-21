/*
 * A060903  a(n) = floor(6*n*sqrt(n)/Pi^2).
 * https://oeis.org/A060903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(6*n*sqrt(n)/Pi^2)
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * Integer square root of a 128‑bit unsigned integer.
 * Returns the largest x such that x*x <= v.
 */
static idx_t int_sqrt_u128(__uint128_t v)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 63);   /* sqrt(2^128-1) < 2^64 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq == v)
            return mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/*
 * Computes a(n) = floor(6*n*sqrt(n)/Pi^2) using only integer arithmetic.
 */
seq_t a(idx_t n)
{
    /* High‑precision rational approximation of 6/Pi^2: 
     * NUM / DEN ≈ 0.6079271018540267
     * Here we use NUM = 6079271, DEN = 10000000 (error < 5e-7) */
    const idx_t NUM = 6079271ULL;
    const idx_t DEN = 10000000ULL;

    /* Scaling factor to obtain an integer approximation of sqrt(n) */
    const idx_t SCALE = 1000000000ULL;   /* 1e9 */

    /* Compute s_scaled = floor( sqrt(n) * SCALE ) without overflow.
     * We find the largest integer s_scaled such that s_scaled^2 <= n * SCALE^2
     * Using 128‑bit arithmetic so the intermediate does not overflow. */
    __uint128_t n_scaled_sq = (__uint128_t)n * (__uint128_t)SCALE * (__uint128_t)SCALE;
    idx_t s_scaled = int_sqrt_u128(n_scaled_sq);

    /* Numerator = NUM * n * s_scaled
     * Denominator = DEN * SCALE
     * The division is exact floor because all operands are integers. */
    __uint128_t num = (__uint128_t)NUM * (__uint128_t)n * (__uint128_t)s_scaled;
    __uint128_t den = (__uint128_t)DEN * (__uint128_t)SCALE;
    seq_t result = (seq_t)(num / den);

    return result;
}
