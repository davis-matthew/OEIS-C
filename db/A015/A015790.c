/*
 * A015790  Inverse of 1781st cyclotomic polynomial.
 * https://oeis.org/A015790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the n-th coefficient of the power‑series expansion of
 *            the reciprocal of the 1781‑st cyclotomic polynomial. The
 *            sequence is periodic with period 1781
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All necessary headers are allowed; seq_t and idx_t are defined elsewhere. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Constants derived from n = 1781 = 13 * 137. */
    enum {
        PERIOD   = 1781,      /* sequence period length   */
        DEN_DEG  = 150,       /* degree of the denominator polynomial (x^150 - x^137 - x^13 + 1) */
        NUM_DEG  = 1782,      /* degree of the numerator polynomial (x^1782 - x^1781 - x + 1)   */
        PHI_DEG  = NUM_DEG - DEN_DEG   /* degree of Φ1781 = 1632 */
    };

    /* Static storage: computed once, reused thereafter. */
    static bool initialized = false;
    static seq_t phi[PHI_DEG + 1];
    static seq_t period[PERIOD];

    if (!initialized) {
        /* Denominator polynomial D(x) = (x^13 - 1)(x^137 - 1)
           = 1 - x^13 - x^137 + x^150. */
        static seq_t D[DEN_DEG + 1] = {0};
        D[0] = 1;
        D[13] = -1;
        D[137] = -1;
        D[150] = 1;

        /* Numerator polynomial N(x) = (x - 1)(x^1781 - 1)
           = x^1782 - x^1781 - x + 1. */
        static seq_t N[NUM_DEG + 1] = {0};
        N[0] = 1;
        N[1] = -1;
        N[1781] = -1;
        N[1782] = 1;

        /* Long division: N(x) = D(x) * Φ1781(x).  Because D[0] == 1,
           the division is performed term‑by‑term. */
        for (idx_t k = 0; k <= PHI_DEG; ++k) {
            seq_t sum = 0;
            /* Sum over i = 1 .. min(k, DEN_DEG) of D[i] * phi[k-i] */
            for (idx_t i = 1; i <= DEN_DEG && i <= k; ++i) {
                sum += (seq_t)D[i] * phi[k - i];
            }
            phi[k] = N[k] - sum;
        }

        /* Compute the first PERIOD terms of the reciprocal series.
           Recurrence: a(0)=1, a(n) = -∑_{i=1}^{min(n,PHI_DEG)} φ_i * a(n-i). */
        period[0] = 1;
        for (idx_t k = 1; k < PERIOD; ++k) {
            seq_t sum = 0;
            idx_t lim = k < PHI_DEG ? k : PHI_DEG;
            for (idx_t i = 1; i <= lim; ++i) {
                sum += phi[i] * period[k - i];
            }
            period[k] = -sum;
        }

        initialized = true;
    }

    /* Return the requested term; the sequence is periodic with LENGTH PERIOD. */
    return period[n % PERIOD];
}
