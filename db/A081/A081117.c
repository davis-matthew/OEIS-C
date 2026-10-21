/*
 * A081117  Differences of Beatty sequence for cube root of 2.
 * https://oeis.org/A081117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*2^(1/3)) - floor(n*2^(1/3))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using integer cube‑root: floor(n*α) = floor(∛(2 · n³)) where α = 2^(1/3). */

#include <stdint.h>
#include <stdbool.h>

/* Integer cube root floor for 0 <= x < 2^128 */
static inline seq_t cube_root_floor(__uint128_t x)
{
    __uint128_t lo = 0, hi = 1;
    /* increase hi until hi³ > x */
    while (hi * hi * hi <= x) hi <<= 1;
    /* binary search between lo and hi */
    while (lo + 1 < hi) {
        __uint128_t mid = (lo + hi) >> 1;
        if (mid * mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

seq_t a(idx_t n)
{
    /* Compute floor((n+1)*α) as floor(∛(2·(n+1)³)) */
    __uint128_t np = (__uint128_t)(n + 1);
    __uint128_t xp = np * np * np * 2ULL;      /* (n+1)³ * 2 */
    seq_t up = cube_root_floor(xp);

    /* Compute floor(n*α) as floor(∛(2·n³)) */
    __uint128_t np0 = (__uint128_t)n;
    __uint128_t x0 = np0 * np0 * np0 * 2ULL;    /* n³ * 2 */
    seq_t low = cube_root_floor(x0);

    return up - low;   /* difference is 1 or 2 */
}
