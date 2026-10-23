/*
 * A171567  Riordan array (f(x), x*f(x)) where f(x) is the g.f. of A168491.
 * https://oeis.org/A171567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,k) = (-1)^(r-k) * (k+1) * binomial(2*r-k, r-k) /
 *            (r+1) for n >= 0. The function uses only integer arithmetic and
 *            the helper `binomial(idx_t, idx_t)` from the utils library
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (true) {
        /* use 128-bit to avoid overflow of (r+1)*(r+2) */
        __uint128_t t = (__uint128_t)(r + 1) * (r + 2) / 2;
        if (t > n)
            break;
        ++r;
    }

    /* column index within row r */
    idx_t k = n - (__uint128_t)r * (r + 1) / 2;

    /* binomial coefficient: C(2r-k, r-k) */
    seq_t bin = binomial((idx_t)(2 * r - k), (idx_t)(r - k));

    /* compute T(r,k) = (k+1)/ (r+1) * bin with sign (−1)^(r‑k) */
    seq_t res = bin * (seq_t)(k + 1);
    res = res / (seq_t)(r + 1);

    if (((r - k) & 1) != 0)   /* multiply by −1 if odd */
        res = -res;

    return res;
}
