/*
 * A102426  Triangle read by rows giving coefficients of polynomials defined
 *          by F(0,x)=0, F(1,x)=1, F(n,x) = F(n-1,x) + x*F(n-2,x).
 * https://oeis.org/A102426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = binom( floor(r/2)+k , floor((r-1)/2)-k ), where n is the
 *            flattened index in the triangle, r is the row number and k is
 *            the position within that row (0‑based). Row lengths: len(0) = 1,
 *            len(r) = ((r-1)/2)+1 for r>0
 * Range:     u8 n <= 44, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   binom_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_val.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* a(0) = 0 */

    idx_t start = 0;                    /* first index of current row */
    idx_t r = 0;                        /* current row */

    /* Locate the row containing n */
    for (;;)
    {
        idx_t len = (r == 0) ? 1 : ((r - 1) / 2) + 1;
        if (start + len > n) break;
        start += len;
        ++r;
    }

    idx_t k = n - start;                /* position in row r */

    /* Binomial parameters according to OEIS formula */
    idx_t a1 = r / 2;                    /* floor(r/2)   */
    idx_t a2 = (r - 1) / 2;              /* floor((r-1)/2) */
    idx_t N = a1 + k;                    /* numerator binom */
    idx_t K = a2 - k;                    /* denominator binom */

    return binom_val(N, K);
}
