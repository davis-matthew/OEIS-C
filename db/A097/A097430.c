/*
 * A097430  Integer part of the radii of circles with area n.
 * https://oeis.org/A097430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(n / pi))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stddef.h>

/* Constants for rational bounds of π
 * SCALE = 1e9   (9 decimal places)
 * LNUM/SCALE < π < UNUM/SCALE
 *   LNUM = floor(π * SCALE) = 3141592653
 *   UNUM = LNUM + 1         = 3141592654
 */
static const uint64_t SCALE     = 1000000000ULL;
static const uint64_t PI_LOWER  = 3141592653ULL; /* floor(π * SCALE)   */
static const uint64_t PI_UPPER  = 3141592654ULL; /* floor(π * SCALE)+1 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Initial guess using long double */
    const long double pi = 3.141592653589793238462643383279502884L;
    idx_t r = (idx_t)floorl(sqrtl((long double)n / pi));

    /* Adjust upwards while the next candidate is still valid.
       (r+1)^2 * PI_UPPER <= n * SCALE  ⇒  (r+1)² ≤ n / π */
    while (1) {
        __uint128_t lhs = (__uint128_t)(r + 1) * (r + 1) * PI_UPPER;
        __uint128_t rhs = (__uint128_t)n * SCALE;
        if (lhs <= rhs)
            r++;
        else
            break;
    }

    /* Adjust downwards if the current candidate is too large.
       r^2 * PI_LOWER > n * SCALE  ⇒  r² > n / π */
    while (1) {
        __uint128_t lhs = (__uint128_t)r * r * PI_LOWER;
        __uint128_t rhs = (__uint128_t)n * SCALE;
        if (lhs > rhs)
            r--;
        else
            break;
    }

    return (seq_t)r;
}
