/*
 * A011646  93rd cyclotomic polynomial.
 * https://oeis.org/A011646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A011646: coefficient of x^n in the 93rd cyclotomic polynomial Φ_93(x). */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Φ_93(x) has degree φ(93)=60. */
    static seq_t coeff[61];
    static bool ready = false;

    if (!ready)
    {
        /* P(x) = (x^93 - 1)(x - 1) = x^94 - x^93 - x + 1 */
        seq_t P[95];
        for (idx_t i = 0; i < 95; ++i) P[i] = (seq_t)0;
        P[94] = (seq_t)1;
        P[93] = (seq_t)-1;
        P[1]  = (seq_t)-1;
        P[0]  = (seq_t)1;

        /* Q(x) = (x^3 - 1)(x^31 - 1) = x^34 - x^31 - x^3 + 1 */
        seq_t Q[35];
        for (idx_t i = 0; i < 35; ++i) Q[i] = (seq_t)0;
        Q[34] = (seq_t)1;
        Q[31] = (seq_t)-1;
        Q[3]  = (seq_t)-1;
        Q[0]  = (seq_t)1;

        /* Long division: P(x) / Q(x) → Φ_93(x) */
        seq_t R[95];
        for (idx_t i = 0; i < 95; ++i) R[i] = P[i];

        const idx_t degP = 94;
        const idx_t degQ = 34;

        for (idx_t i = degP; i >= degQ; --i)
        {
            seq_t coef = R[i];
            idx_t k = i - degQ;          /* exponent of quotient term   */
            coeff[k] = coef;
            if (coef != (seq_t)0)
            {
                for (idx_t j = 0; j <= degQ; ++j)
                    R[k + j] -= coef * Q[j];
            }
        }
        ready = true;
    }

    if (n <= 60)
        return coeff[(size_t)n];
    else
        return (seq_t)0;
}
