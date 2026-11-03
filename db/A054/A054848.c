/*
 * A054848  n - nearest integer to (nearest integer to n^(2/3))^(3/2).
 * https://oeis.org/A054848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n - nearest integer to sqrt( (round(n^(2/3)))^3 ) where
 *            round(x) = nearest integer to x Using integer arithmetic only
 * Range:     i8 n <= 3, i16 n <= 63, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

/* Nearest integer to square root of x (rounding half up) */
static inline seq_t round_root2(seq_t x)
{
    seq_t m = floor_sqrt(x);         /* floor sqrt */
    seq_t m2 = m * m;
    /* Compare 4*x with 4*m^2 + 4*m + 1  (half point) */
    seq_t lhs = 4 * x;
    seq_t rhs = 4 * m2 + 4 * m + 1;
    return (lhs < rhs) ? m : m + 1;
}

/* Nearest integer to cube root of x (rounding half up) */
static inline seq_t round_root3(seq_t x)
{
    seq_t m = floor_cube_root(x);     /* floor cube root */
    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    /* Compare 8*x with 8*m^3 + 12*m^2 + 6*m + 1  (half point) */
    seq_t lhs = 8 * x;
    seq_t rhs = 8 * m3 + 12 * m2 + 6 * m + 1;
    return (lhs < rhs) ? m : m + 1;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;        /* cast index to full precision type */

    /* r1 = round( n^(2/3) )  ≡ round( (n^2)^(1/3) ) */
    seq_t N2 = N * N;
    seq_t r1 = round_root3(N2);

    /* r2 = round( sqrt( r1^3 ) ) */
    seq_t r1_3 = r1 * r1 * r1;
    seq_t r2 = round_root2(r1_3);

    return N - r2;   /* a(n) = n – r2 */
}
