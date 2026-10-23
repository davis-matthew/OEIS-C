/*
 * A158880  Number of spanning trees in C_6 X P_n.
 * https://oeis.org/A158880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*U(n-1,3/2)^2*U(n-1,5/2)^2*U(n-1,3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   calc_U
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_U.h"

seq_t a(idx_t n)
{
    /* n must be >= 1 (offset 1) */
    if (n < 1) return (seq_t)0;         /* undefined case, but safe */

    idx_t m = n - 1;                    /* index for the Chebyshev polynomials */

    /* coefficients 2x for the three needed polynomials */
    const seq_t coeff_3_2 = (seq_t)3;    /* 2*(3/2) = 3  */
    const seq_t coeff_5_2 = (seq_t)5;    /* 2*(5/2) = 5  */
    const seq_t coeff_3   = (seq_t)6;    /* 2*3     = 6  */

    seq_t u3  = calc_U(m, coeff_3_2);
    seq_t u5  = calc_U(m, coeff_5_2);
    seq_t u6  = calc_U(m, coeff_3);

    /* compute 6 * u3^2 * u5^2 * u6 */
    seq_t result = (seq_t)6;
    result *= u3;       /* multiply by u3 */
    result *= u3;       /* another u3 (squared) */
    result *= u5;       /* first u5 */
    result *= u5;       /* second u5 (squared) */
    result *= u6;       /* final u6 */

    return result;
}
