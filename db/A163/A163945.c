/*
 * A163945  Triangle interpolating between (-1)^n (A033999) and the swinging
 *          factorial function (A056040) restricted to odd indices (2n+1)$
 *          (A002457), read by rows.
 * https://oeis.org/A163945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   * a(n) = (-1)^(r-k) * C(r,k) * (2k+1)! / (k!^2) * where r and k
 *            are the row and column indices of n in the * triangle T(n,k)
 *            read by rows
 * Range:     i8 n <= 8, i16 n <= 32, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t left = 0, right = n, r = 0;
    while (left <= right)
    {
        idx_t mid = (left + right) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n)
        {
            r = mid;
            left = mid + 1;
        }
        else
            right = mid - 1;
    }

    /* Column k within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute the product of binomial coefficients */
    seq_t bin1 = binomial(r, k);                 /* C(r,k)   */
    seq_t bin2 = binomial(2 * k + 1, k);          /* C(2k+1,k)*/
    seq_t term  = bin1 * bin2;                    /* C(r,k)*C(2k+1,k) */

    /* Multiply by (k+1) and apply the sign */
    term = term * ((seq_t)(k + 1));
    if ((r - k) & 1)
        term = -term;

    return term;
}
