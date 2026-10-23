/*
 * A011644  85th cyclotomic polynomial.
 * https://oeis.org/A011644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 85th cyclotomic polynomial
 *            Φ85(x) = (x^85 - 1)(x - 1) / ((x^5 - 1)(x^17 - 1)) (0 ≤ n ≤ 64)
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* coefficients for degrees 0..64, initialized once */
    static seq_t coeff[65];
    static bool init = false;

    if (!init) {
        /* numerator = (x^85 - 1)(x - 1) = x^86 - x^85 - x + 1 */
        seq_t num[87] = {0};
        num[0] =  1;   /* +1 */
        num[1] = -1;   /* -x   */
        num[85] = -1;  /* -x^85*/
        num[86] =  1;  /* +x^86*/

        /* denominator = (x^5 - 1)(x^17 - 1) = x^22 - x^17 - x^5 + 1 */
        seq_t den[23] = {0};
        den[0]  =  1;   /* +1 */
        den[5]  = -1;   /* -x^5 */
        den[17] = -1;   /* -x^17 */
        den[22] =  1;   /* +x^22 */

        /* polynomial long division: num / den  (den has leading coeff = 1) */
        for (int i = 86; i >= 22; --i) {
            seq_t q = num[i];         /* quotient coefficient for degree i-22 */
            coeff[i - 22] = q;        /* store coefficient in the result array */
            /* subtract q * (den shifted by i-22) from num */
            for (int j = 0; j <= 22; ++j) {
                num[i - 22 + j] -= q * den[j];
            }
        }
        /* all remaining num[0..21] should be zero (remainder) */
        init = true;
    }

    if (n <= 64) {
        return coeff[n];
    } else {
        /* coefficients beyond the polynomial degree are zero */
        return 0;
    }
}
