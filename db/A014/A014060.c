/*
 * A014060  Inverse of 51st cyclotomic polynomial.
 * https://oeis.org/A014060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series of 1/Φ₅₁(x); the
 *            sequence is periodic with period 51
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define PERIOD 51
#define DEG    32

seq_t a(idx_t n)
{
    static seq_t philox[DEG + 1];
    static seq_t table[PERIOD];
    static int initialized = 0;

    if (!initialized) {
        /* Step 1: compute Φ₅₁(x) as the quotient of
           (x⁵¹-1)(x-1) divided by (x¹⁷-1)(x³-1).          */
        seq_t num[53] = {0};
        num[52] = 1;     /* x⁵² */
        num[51] = -1;    /* -x⁵¹ */
        num[1]  = -1;    /* -x   */
        num[0]  = 1;     /* +1   */

        seq_t den[21] = {0};
        den[20] = 1;     /* x²⁰ */
        den[17] = -1;    /* -x¹⁷ */
        den[3]  = -1;    /* -x³   */
        den[0]  = 1;     /* +1    */

        seq_t temp[53];
        for (size_t i = 0; i < 53; ++i) temp[i] = num[i];

        /* Long division: dividend 'temp', divisor 'den' (leading coeff 1). */
        for (int i = 52; i >= 20; --i) {
            seq_t coeff = temp[i];
            if (coeff == 0) continue;
            size_t shift = (size_t)(i - 20);
            philox[shift] = coeff;
            /* temp -= coeff * den shifted by 'shift' */
            for (size_t j = 0; j <= 20; ++j)
                temp[shift + j] -= coeff * den[j];
        }
        /* The remaining coefficients of Φ₅₁(x) are in philox[0..DEG] */

        /* Step 2: recurrence for the inverse series: a(0)=1, */
        /* a(k) = - Σ_{j=1}^{DEG} Φ₅₁(j) * a(k-j)   for k>0    */
        table[0] = 1;
        for (size_t k = 1; k < PERIOD; ++k) {
            seq_t sum = 0;
            for (size_t j = 1; j <= DEG && j <= k; ++j)
                sum += philox[j] * table[k - j];
            table[k] = -sum;
        }
        initialized = 1;
    }

    return table[(size_t)(n % PERIOD)];
}
