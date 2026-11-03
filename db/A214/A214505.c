/*
 * A214505  a(n) = 1 if n is four times a triangular number, -1 if one more
 *          than twelve times a triangular number else 0.
 * https://oeis.org/A214505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n is four times a triangular number, * -1 if n is
 *            one more than twelve times a triangular number, * else 0, where
 *            T_k = k*(k+1)/2. * The implementation checks whether n/4 or
 *            (n-1)/12 is triangular * by verifying that 1+8*m is a perfect
 *            square
 * Range:     i8 n <= 31, i16 n <= 8121, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 101 terms through n = 9363 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

/* Helper: check if m is a triangular number.
 * Returns true iff m = k*(k+1)/2 for some integer k >= 0. */
static bool is_triangular(seq_t m)
{
    /* D = 1 + 8*m = (2k+1)^2 for triangular m. */
    seq_t D = (seq_t)1 + (seq_t)8 * m;
    seq_t sqrtD = floor_sqrt(D);
    return sqrtD * sqrtD == D;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* Check if n == 4 * T_k */
    if (n % 4 == 0) {
        seq_t m = sn / 4;
        if (is_triangular(m))
            return (seq_t)1;
    }

    /* Check if n == 12 * T_k + 1 */
    if (n >= 1 && (n - 1) % 12 == 0) {
        seq_t m = (sn - 1) / 12;
        if (is_triangular(m))
            return (seq_t)-1;
    }

    return (seq_t)0;
}
