/*
 * A124788  Triangle read by rows: expansion of (1+x*y)/(1-x^2*y^2-x^3*y^2).
 * https://oeis.org/A124788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle * T(r,k) =
 *            binomial(floor(k/2), r-k) row‑by‑row. * The flat index n
 *            satisfies r(r+1)/2 <= n < (r+1)(r+2)/2; k = n - r(r+1)/2
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t t = 0;       /* cumulative number of terms before current row */
    idx_t r = 0;       /* current row index */

    /* Find the row r containing the n-th term */
    while (1) {
        idx_t row_len = r + 1;            /* length of row r */
        if (t + row_len > n) break;       /* n is in this row */
        t += row_len;                      /* move to next row */
        r++;
    }

    idx_t k = n - t;                       /* column index within row r */
    idx_t floor_k2 = k >> 1;               /* floor(k/2) */
    idx_t nk = r - k;                      /* r - k */

    if (floor_k2 < nk)                     /* binomial would be zero */
        return (seq_t)0;

    return binom(floor_k2, nk);            /* compute binomial coefficient */
}
