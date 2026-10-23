/*
 * A011654  119th cyclotomic polynomial.
 * https://oeis.org/A011654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in Φ_119(x) Φ_119(x) = Φ_7(x^17) /
 *            Φ_7(x) where Φ_7(x) = 1 + x + … + x^6
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the 97‑term coefficient array on the first
   call, then returns the requested term. The algorithm uses only
   integer arithmetic and works for any seq_t/idx_t type supplied. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t coeff[97];         /* coefficients for degrees 0..96 */

    if (!initialized) {
        /* Denominator polynomial Φ_7(x) = 1 + x + ... + x^6  (degree 6) */
        const int denom_deg = 6;
        /* Build numerator Φ_7(x^17)  (degree 6*17 = 102) */
        seq_t num[103] = {0};  /* indices 0..102 */
        for (int e = 0; e <= denom_deg; ++e)
            num[e * 17] = 1;   /* coefficient 1 at exponent 17*e */

        /* Initialize coefficient array to zero */
        for (int i = 0; i < 97; ++i)
            coeff[i] = 0;

        /* Polynomial long division: num / (1 + x + ... + x^6) */
        /* Since leading coefficient of denominator is 1, we can proceed
           from the highest degree term downwards. */
        for (int i = 102; i >= denom_deg; --i) {
            seq_t coef = num[i];
            if (coef != 0) {
                int qpos = i - denom_deg;   /* position in quotient */
                coeff[qpos] = coef;         /* store quotient coefficient */
                /* subtract coef * denominator shifted by qpos */
                for (int j = 0; j <= denom_deg; ++j)
                    num[qpos + j] -= coef;  /* denom coefficient is 1 */
            }
        }
        /* The remainder should be zero here; any non‑zero would indicate
           a programming error for this special case. */
        initialized = true;
    }

    if (n >= 97)
        return 0;  /* coefficients beyond degree 96 are zero for Φ_119 */

    return coeff[(size_t)n];
}
