/*
 * A091764  Triangle of coefficients of n-th degree interpolating polynomial
 *          to sqrt(x) multiplied by 4^n.
 * https://oeis.org/A091764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the flattened row‑major listing of the triangle T(r,k)
 *            defined for r ≥ 0, 0 ≤ k ≤ r by T(r,0) = binom(2r, r) T(r,k) =
 *            binom(2r, r) * binom(r, k) * (-1)^{k-1} / (2k-1) for k ≥ 1 Here
 *            n is the linear index, r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r*(r+1)/2
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* find the row r and column k corresponding to index n */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* correct possible round‑off errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;          /* column number */

    seq_t central = binom(2 * r, r);     /* binom(2r,r) */

    if (k == 0)
        return central;                 /* first element of the row */

    seq_t bin = binom(r, k);            /* binom(r,k) */
    idx_t denom = 2 * k - 1;            /* odd positive integer */

    /* use 128‑bit arithmetic to avoid overflow for 64‑bit builds   */
    __int128 prod = (__int128)central * (__int128)bin;
    seq_t coeff = (seq_t)(prod / (__int128)denom);

    /* apply the sign factor (-1)^{k-1} for k≥1 */
    if ((k - 1) & 1)
        coeff = -coeff;

    return coeff;
}
