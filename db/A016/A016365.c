/*
 * A016365  253rd cyclotomic polynomial.
 * https://oeis.org/A016365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 220, i16 n <= 220, i32 n <= 220, i64 n <= 220,
 *            i128 n <= 220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Cyclotomic 253 coefficients via polynomial division.
   The coefficients of the 253rd cyclotomic polynomial Φ_253(x) are
   computed by the identity
        Φ_253(x) = (x-1)(x^253-1) / ((x^11-1)(x^23-1))
   and a lookup is performed on the resulting integer polynomial.
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The polynomial Φ_253(x) has degree φ(253)=220.
       We build it once on the first call. */
    static seq_t coeff[221];
    static bool initialized = false;

    if (!initialized) {
        /* numerator: (x-1)(x^253-1) = x^254 - x^253 - x + 1 */
        seq_t num[255] = {0};          /* indices 0..254 */
        num[0] = 1;                    /* +1 */
        num[1] = -1;                   /* -x */
        num[253] = -1;                 /* -x^253 */
        num[254] = 1;                  /* +x^254 */

        /* denominator: (x^11-1)(x^23-1) = x^34 - x^23 - x^11 + 1 */
        seq_t denom[35] = {0};         /* indices 0..34 */
        denom[0] = 1;                  /* +1 */
        denom[11] = -1;                /* -x^11 */
        denom[23] = -1;                /* -x^23 */
        denom[34] = 1;                 /* +x^34 */

        /* Polynomial long division: num / denom */
        /* The quotient has degree 220, array size 221 */
        for (idx_t i = 254; i >= 34; --i) {
            seq_t c = num[i];          /* leading coeff of the current remainder slice */
            if (c == 0) continue;
            coeff[i - 34] = c;         /* store quotient coefficient */

            /* subtract c * denom shifted by (i-34) from num */
            for (idx_t j = 0; j <= 34; ++j) {
                num[(i - 34) + j] -= c * denom[j];
            }
            /* after subtraction, num[i] should become 0 automatically */
        }

        /* Anything remaining beyond degree 220 (i < 34) must be zero
           because division is exact. */
        for (idx_t k = 0; k < 34; ++k) {
            if (num[k] != 0) {
                /* In very unlikely event numeric error, fallback: */
                coeff[k] = num[k];
            }
        }

        initialized = true;
    }

    if (n >= 221) return (seq_t)0;
    return coeff[n];
}
