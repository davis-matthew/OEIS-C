/*
 * A008533  Coordination sequence for {A_5}* lattice.
 * https://oeis.org/A008533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) -10*a(n-2) +10*a(n-3) -5*a(n-4) +a(n-5), a(0)=1,
 *            a(1)=12, a(2)=72, a(3)=272, a(4)=762, a(5)=1752. Computed via
 *            the generating function (1+x)(1+6x+16x^2+6x^3+x^4)/(1-x)^5 which
 *            reduces to a(n) = Σ_{i=0}^{5} c[i]·C(n-i+4,4) with coefficients
 *            c = {1,7,22,22,7,1}
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 203, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial (1+x)(1+6x+16x^2+6x^3+x^4) */
    static const seq_t coeff[6] = {1, 7, 22, 22, 7, 1};
    seq_t res = 0;
    for (idx_t i = 0; i <= 5 && i <= n; ++i) {
        idx_t m = n - i + 4;                 /* m >= 4 for i <= n */
        /* compute C(m,4) = m*(m-1)*(m-2)*(m-3)/24 */
        seq_t c = (seq_t)m * (m - 1) * (m - 2) * (m - 3) / 24;
        res += coeff[i] * c;
    }
    return res;
}
