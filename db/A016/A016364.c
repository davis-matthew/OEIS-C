/*
 * A016364  247th cyclotomic polynomial.
 * https://oeis.org/A016364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 216, i16 n <= 216, i32 n <= 216, i64 n <= 216,
 *            i128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of the 247th cyclotomic polynomial Φ_247(x) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* There are 217 coefficients, from degree 0 to 216. */
    static seq_t coef[217];
    static bool ready = false;

    if (!ready) {
        /* Numerator: Σ_{k=0}^{246} x^k  (coefficients all 1) */
        seq_t numer[247] = {0};
        for (idx_t i = 0; i <= 246; i++)
            numer[i] = (seq_t)1;

        /* Denominator: (1 + x + … + x^12) * (1 + x + … + x^18) */
        /* Its coefficients are the convolution of two sequences of ones. */
        seq_t denom[31] = {0};
        for (idx_t i = 0; i <= 12; i++)
            for (idx_t j = 0; j <= 18; j++)
                denom[i + j] += (seq_t)1;   /* ranges 0 … 30 */

        /* Long division numerator / denom, storing the quotient in coef[]. */
        seq_t quot[217] = {0};
        for (idx_t i = 246; i >= 30; --i) {
            seq_t coeff = numer[i];          /* leading coefficient (always 1) */
            quot[i - 30] = coeff;           /* place in quotient */
            if (coeff != (seq_t)0) {
                /* subtract coeff * denom * x^{i-30} from numer */
                for (idx_t j = 0; j <= 30; ++j)
                    numer[i - 30 + j] -= coeff * denom[j];
            }
        }

        /* Copy the quotient to the static cache. */
        for (idx_t i = 0; i < 217; ++i)
            coef[i] = quot[i];

        ready = true;
    }

    return (n < 217) ? coef[n] : (seq_t)0;
}
