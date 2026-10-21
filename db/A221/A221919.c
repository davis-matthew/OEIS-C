/*
 * A221919  Triangle of numerators of sum of two unit fractions: 1/n + 1/m, n
 *          >= m >= 1.
 * https://oeis.org/A221919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) = numerator(1/r + 1/m), where r and m are found by *
 *            flattening the triangle with rows r=1,2,... and columns m=1..r.
 *            * For index n, find the smallest r with r*(r+1)/2 >= n; then * m
 *            = n - (r-1)*r/2. The numerator equals (r+m) / gcd(r+m, r*m)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __uint128_t gcd128(__uint128_t a, __uint128_t b)
{
    while (b != 0) {
        __uint128_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* previous triangular number T(r-1) = (r-1)*r/2 */
    __uint128_t prev_tri = (__uint128_t)(r - 1) * r / 2;
    idx_t m = n - (idx_t)prev_tri;

    /* compute numerator = (r+m)/gcd(r+m, r*m) */
    __uint128_t A = (__uint128_t)r + (__uint128_t)m; /* r + m */
    __uint128_t B = (__uint128_t)r * (__uint128_t)m; /* r * m */

    __uint128_t g = gcd128(A, B);
    __uint128_t num = A / g;

    return (seq_t)num;
}
