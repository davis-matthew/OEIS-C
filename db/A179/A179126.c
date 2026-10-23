/*
 * A179126  Positive integers m for which the torsion subgroup of the elliptic
 *          curve y^2 = x^3 + m has order 3.
 * https://oeis.org/A179126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k² where k is the nth positive integer that is not a
 *            perfect cube
 * Range:     u8 n <= 13, u16 n <= 249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute the floor of the integer cube root of x.
   Uses binary search with 128‑bit arithmetic to avoid overflow. */
static inline idx_t _floor_cuberoot(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = 1;
    /* Expand hi until hi³ > x */
    while ((__uint128_t)hi * hi * hi <= x)
        hi <<= 1;
    /* Binary search for maximum r with r³ ≤ x */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __uint128_t m3 = (__uint128_t)mid * mid * mid;
        if (m3 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Test whether k is a perfect cube. */
static inline bool _is_cube(idx_t k)
{
    idx_t r = _floor_cuberoot(k);
    return ((__uint128_t)r * r * r) == k;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for 0, return 0 as a safety value */
        return 0;

    idx_t count = 0;   /* how many squares found so far */
    idx_t k = 1;       /* current integer to test */

    while (1) {
        ++k;
        if (_is_cube(k))
            continue;   /* skip cubes */

        ++count;       /* k is not a cube, its square belongs to the sequence */
        if (count == n) {
            seq_t kk = (seq_t)k;
            return kk * kk;
        }
    }
}
