/*
 * A016358  217th cyclotomic polynomial.
 * https://oeis.org/A016358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 217‑th cyclotomic polynomial
 *            Φ_217(x)
 * Range:     i8 n <= 180, i16 n <= 180, i32 n <= 180, i64 n <= 180,
 *            i128 n <= 180, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    enum { DEG = 180, NUM_DEG = 218, DEN_DEG = 38 };

    static seq_t coeff[DEG + 1];
    static int initialized = 0;

    if (!initialized) {
        static seq_t numer[NUM_DEG + 1];
        static seq_t denom[DEN_DEG + 1];
        static seq_t quotient[DEG + 1];

        /* set numerator coefficients for (x^217 - 1)(x - 1) */
        numer[218] =  1;
        numer[217] = -1;
        numer[1]   = -1;
        numer[0]   =  1;

        /* set denominator coefficients for (x^31 - 1)(x^7 - 1) */
        denom[38] =  1;
        denom[31] = -1;
        denom[7]  = -1;
        denom[0]  =  1;

        /* polynomial long division: numer = denom * quotient */
        for (idx_t i = NUM_DEG;; --i) {
            if (i < DEN_DEG) break;
            seq_t coef = numer[i];               /* denom[0] == 1 */
            quotient[i - DEN_DEG] = coef;
            for (idx_t j = 0; j <= DEN_DEG; ++j) {
                numer[i - DEN_DEG + j] -= coef * denom[j];
            }
        }

        /* store result in coeff array */
        for (idx_t i = 0; i <= DEG; ++i) {
            coeff[i] = quotient[i];
        }

        initialized = 1;
    }

    if (n <= DEG) {
        return coeff[n];
    } else {
        return (seq_t)0;
    }
}
