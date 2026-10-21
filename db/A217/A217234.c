/*
 * A217234  Triangle of expansion coefficients of the sum of an n X n array
 *          with equal top row and left column (extended by the rule of
 *          Pascal's triangle) in terms of the top row elements.
 * https://oeis.org/A217234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficients T(r,c) of the sum of an r×r Pascal array *
 *            with identical top row and left column. * For r >= 2 and c >= 2:
 *            * T(r,c) = 2 * binom(2*r - c, r - 1) * For c == 1: T(r,1) = 1.
 *            (Also T(1,1) = 1)
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

#include <stdint.h>

/* Integer square‑root of an unsigned 64‑bit value.  */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;
}

/* Return the a(n) value for the OEIS sequence A217234.  */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n  (1‑based) */
    idx_t r = (isqrt_u64(8*n + 1) - 1) >> 1;
    /* Adjust if the triangular number is too small */
    __uint128_t tri = (__uint128_t)r * (r + 1) / 2;
    if (tri < n)
        ++r;

    /* Column c (1‑based) within row r */
    __uint128_t prev_tri = (__uint128_t)r * (r - 1) / 2;
    idx_t c = (idx_t)(n - prev_tri);

    /* Handle the first column explicitly */
    if (c == 1)
        return (seq_t)1;

    /* For r >= 2 and c >= 2 use the closed form:
     *     T(r,c) = 2 * binom(2*r - c, r - 1)
     */
    idx_t a_param = 2 * r - c;
    idx_t b_param = r - 1;
    seq_t bin = binom(a_param, b_param);
    return bin * (seq_t)2;
}
