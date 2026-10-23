/*
 * A168619  Triangle T(n,k) read by rows with the coefficient [x^k] of the
 *          polynomial (x+1)^n + (2*n-3) *( (x+1)^n -x^n -1 ) in column k, row
 *          n.
 * https://oeis.org/A168619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑wise enumeration of the triangle * with entries
 *            * * T(r,k) = 1 if k = 0 or k = r * (2r-2) * C(r,k) otherwise * *
 *            where C denotes the binomial coefficient. * The sequence is the
 *            list T(0,0), T(1,0),T(1,1), T(2,0),T(2,1),T(2,2), ... * * The
 *            implementation finds the row r and column...
 * Range:     u8 n <= 30, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t cum_before = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - cum_before;            /* column within the row */

    /* endpoints are always 1 */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* interior coefficients */
    seq_t A = (seq_t)(2 * r - 2);            /* 2r-2 */
    seq_t bin = binomial(r, k);              /* C(r,k) */
    return A * bin;
}
