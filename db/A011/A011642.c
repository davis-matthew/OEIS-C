/*
 * A011642  77th cyclotomic polynomial.
 * https://oeis.org/A011642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 77th cyclotomic polynomial
 *            Φ_77(x) which equals Φ_7(x^11)/Φ_7(x). The degree of Φ_77 is 60,
 *            so a(n)=0 for n>60
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t coeff[61];
    static bool ready = false;

    /* We only need to build the polynomial once. */
    if (!ready) {
        /* Temporary arrays for the long‑division.  Their sizes are fixed. */
        static seq_t num[67];   /* degree 66 polynomial Φ_7(x^11) */
        static seq_t den[7];    /* degree 6 polynomial Φ_7(x)  */

        /* Φ_7(x^11) : 1 + x^11 + x^22 + … + x^66 */
        for (int k = 0; k < 7; ++k)
            num[11 * k] = 1;

        /* Φ_7(x) : 1 + x + … + x^6 */
        for (int i = 0; i < 7; ++i)
            den[i] = 1;

        /* Long division:  Φ_7(x^11) / Φ_7(x)  →  coeff */
        for (int i = 66; i >= 6; --i) {
            seq_t qcoeff = num[i];
            int idx = i - 6;           /* position in the quotient */
            coeff[(idx_t)idx] = qcoeff;

            if (qcoeff != 0) {
                for (int j = 0; j < 7; ++j) {
                    num[idx + j] -= qcoeff * den[j];
                }
            }
        }

        ready = true;
    }

    if (n < 61)
        return coeff[(idx_t)n];

    /* Coefficients beyond the degree of Φ_77 are zero. */
    return 0;
}
