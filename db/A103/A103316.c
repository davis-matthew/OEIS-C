/*
 * A103316  Riordan array (1/(1+2x), x/(1+x)).
 * https://oeis.org/A103316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) enumerates the flattened triangle T(n,k) defined by T(n,k)
 *            = (-1)^(n-k) * Sum_{j=k}^n binomial(n,j) with T(0,0)=1;
 *            T(1,0)=-2; T(1,1)=1. a(n) = T(r,c) where r is the row such that
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2 and c = n - r(r+1)/2
 * Range:     i8 n <= 35, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* determine row r from index n */
    idx_t r = (int_sqrt(8 * n + 1) - 1) / 2;
    /* correct possible off‑by‑one due to integer sqrt */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* column c in that row */
    idx_t c = n - r * (r + 1) / 2;

    /* compute 2^r as pow2 */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < r; ++i)
        pow2 = pow2 * (seq_t)2;

    /* prefix sum of binomial coefficients for j = 0 .. c-1 */
    seq_t prefix = 0;
    seq_t bin = 1;                     /* bin = C(r,0) */
    for (idx_t j = 0; j < c; ++j) {
        prefix += bin;
        /* update bin to C(r,j+1) */
        bin = bin * (seq_t)(r - j) / (seq_t)(j + 1);
    }

    /* T(r,c) = (-1)^(r-c) * (2^r - prefix) */
    seq_t sum = pow2 - prefix;
    if (((r - c) & 1) != 0)
        sum = -sum;
    return sum;
}
