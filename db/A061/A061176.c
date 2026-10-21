/*
 * A061176  Coefficients of polynomials ( (1 -x +sqrt(x))^n + (1 -x
 *          -sqrt(x))^n )/2.
 * https://oeis.org/A061176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = coefficient of x^k in the concatenated polynomials * ((1
 *            -x +sqrt(x))^r + (1 -x -sqrt(x))^r)/2, where the r-th polynomial
 *            * has degree r and contains r+1 coefficients. The index n
 *            selects the * coefficient reading off the flattened list of all
 *            polynomials
 * Range:     i8 n <= 37, i16 n <= 159, i32 n <= 508, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>

/* Integer square‑root: floor(sqrt(x)) for unsigned 64‑bit x */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute T(n,k) = Σ_{j=0..k} (-1)^(k-j) * C(n,2j) * C(n-2j, k-j)   */
static inline seq_t T_value(idx_t n, idx_t k)
{
    seq_t result = 0;
    /* Maximum j is min(k, floor(n/2)) */
    idx_t max_j = (n < (k << 1)) ? n >> 1 : k;
    for (idx_t j = 0; j <= max_j; ++j)
    {
        idx_t two_j = j << 1;
        seq_t c1 = binomial(n, two_j);
        seq_t c2 = binomial(n - two_j, k - j);
        seq_t term = c1 * c2;
        if (((k - j) & 1) != 0)
            result -= term;
        else
            result += term;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t sq = isqrt_u64(8 * n + 1);
    idx_t r = (sq - 1) >> 1;

    /* Offset within the r‑th polynomial */
    idx_t prev = r * (r + 1) / 2;
    idx_t c = n - prev;

    return T_value(r, c);
}
