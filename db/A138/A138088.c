/*
 * A138088  Triangle read by rows: coefficients of characteristic polynomials
 *          of the Z/nZ addition matrices using PolynomialMod[p(x,n),n] in
 *          Mathematica. ( Polynomials/nZ): P(x, n) = If[Mod[n, 4] == 0, x^n,
 *          If[Mod[n, 2] == 1, If[n == 0, 1, (n - 1)*x^n], (n/2)x^(n - 2) +
 *          x^n]].
 * https://oeis.org/A138088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabf
 * Formula:   a(n) is the n‑th coefficient when the rows of the triangle *
 *            defined by the polynomial * P(x,m) = If[Mod[m,4]==0, x^m, *
 *            If[Mod[m,2]==1, (m-1)*x^m, * (m/2)*x^(m-2)+x^m]] * are read
 *            row‑by‑row. The rows are numbered from m=0. * * a(n) is obtained
 *            by expanding each P(x,m) into its coefficient list...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   coeff_m_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/coeff_m_i.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* offset is 1, but guard against n=0 */

    /* Find the row m that contains the nth coefficient */
    idx_t m = 0;
    idx_t total = 0;
    while (1) {
        idx_t len = (m == 1) ? 1 : (m + 1);   /* row length */
        if (n <= total + len)
            break;
        total += len;
        ++m;
    }
    idx_t pos = n - total - 1;               /* zero‑based position in row */

    return coeff_m_i(m, pos);
}
