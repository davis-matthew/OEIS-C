/*
 * A127675  Coefficient table for Chebyshev's U(2*n,x) polynomials in
 *          decreasing powers of (1-x^2).
 * https://oeis.org/A127675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 28, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula implemented:
    a(idx_t n) returns the nth element of OEIS A127675.
    Numbers are enumerated row‑by‑row from the coefficient table
    a(row,col) = (-4)^(row-col) * binomial(2*row - col, col)
                 * (2*row + 1) / (2*(row-col) + 1),
    where row and col are obtained from the flattened triangle
    index n.  The first few values are
    1, -4, 3, 16, -20, 5, -64, 112, -56, 7, …            */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"

/* integer square root for 64‑bit unsigned values */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* fast exponentiation of (-4)^k */
static seq_t pow_neg4(idx_t k)
{
    seq_t result = 1;
    seq_t base   = -4;
    while (k) {
        if (k & 1) result *= base;
        k >>= 1;
        if (k) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find row such that T(row)=row*(row+1)/2 <= n < T(row+1) */
    long double disc = sqrtl((long double)(8.0L * n + 1.0L));
    idx_t row = (idx_t)((disc - 1.0L) / 2.0L);

    /* correct possible rounding errors */
    __uint128_t start = (__uint128_t)row * (row + 1) / 2;
    while (start > n) {
        row--;
        start = (__uint128_t)row * (row + 1) / 2;
    }
    while ((__uint128_t)(row + 1) * (row + 2) / 2 <= n) {
        row++;
    }

    idx_t col = n - (idx_t)start;           /* column index in row */
    idx_t k   = row - col;                  /* exponent for (-4) */

    seq_t val = binomial(2 * row - col, col);
    val *= (seq_t)(2 * row + 1);
    val /= (seq_t)(2 * k + 1);
    val *= pow_neg4(k);
    return val;
}
