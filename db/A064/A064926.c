/*
 * A064926  Period of continued fraction for sqrt(22)*n.
 * https://oeis.org/A064926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the length of the periodic part of the continued
 * fraction of √22·n by the standard quadratic‑surd algorithm.
 * The period ends when the next partial quotient equals 2·⌊√22·n⌋. */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 ≤ x < 2^64. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Work with 64‑bit integers; the values are small for n ≤ 10 000. */
    uint64_t N = (uint64_t)n;               /* n */
    uint64_t D = 22ull * N * N;             /* D = 22·n²  < 2²³ */
    uint64_t a0 = isqrt_u64(D);             /* floor(√D) = floor(n·√22) */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    idx_t period = 0;

    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period++;
    } while (a != 2 * a0);

    return (seq_t)period;
}
