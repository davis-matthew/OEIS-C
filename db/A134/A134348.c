/*
 * A134348  A007318^(-1) * A134347.
 * https://oeis.org/A134348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the entry of triangle A134347 at row r and * column k
 *            where r(r+1)/2 ≤ n < (r+1)(r+2)/2, k = n - r(r+1)/2. * Triangle
 *            definition: * T(r,r) = 2^(r+1) - 1 * T(r,k) = (-1)^(r-k+1) *
 *            C(r,k) for k < r
 * Range:     i8 n <= 34, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b)      */
#include "../utils/pow2.h"    /* provides seq_t pow2(idx_t exp)              */

seq_t a(idx_t n)
{
    /* locate row r.  r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n)
        ++r;

    /* column index within the row */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* last element of the row: 2^(r+1) – 1  */
    if (k == r) {
        seq_t p = pow2(r + 1);   /* 2^(r+1) */
        return p - 1;
    }

    /* other elements: (-1)^(r-k+1) * C(r,k) */
    seq_t bin = binom(r, k);
    if (((r - k + 1) & 1) != 0) /* odd exponent → negative sign */
        return -bin;
    return bin;
}
