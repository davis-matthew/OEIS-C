/*
 * A178737  Coefficients in expansion of Jacobi theta_1'''(0).
 * https://oeis.org/A178737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 unless n is a triangular number n = k(k+1)/2. When n =
 *            k(k+1)/2, a(n) = (-1)^k * (2k+1)^3
 * Range:     i8 n <= 5, i16 n <= 135, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Validated: standard, pass, 104 terms through n = 4944 (2026-11-01)
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
    seq_t nn = (seq_t)n;
    seq_t eight_n_plus_one = nn * (seq_t)8 + (seq_t)1;
    seq_t t = floor_sqrt(eight_n_plus_one);

    if (t * t != eight_n_plus_one)          /* not a triangular number */
        return (seq_t)0;

    if ((t & (seq_t)1) == (seq_t)0)         /* t must be odd */
        return (seq_t)0;

    seq_t k = (t - (seq_t)1) / (seq_t)2;

    /* optional verification (should always hold when t*t == 8*n+1) */
    seq_t tri = k * (k + (seq_t)1) / (seq_t)2;
    if (tri != nn)
        return (seq_t)0;

    seq_t base = k * (seq_t)2 + (seq_t)1;
    seq_t val = base * base * base;

    if ((k & (seq_t)1) != (seq_t)0)    /* k odd → sign negative */
        return -val;
    return val;
}
