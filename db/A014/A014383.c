/*
 * A014383  Inverse of 374th cyclotomic polynomial.
 * https://oeis.org/A014383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based on the inverse of the 374th cyclotomic polynomial.
   Φ_374(x) = Φ_187(-x), with Φ_187(x) = (x‑1)(x^187‑1)/(x^11‑1)(x^17‑1).
   The coefficients φ_k (k = 0..160) are obtained by exact polynomial division.
   For n > 0:
        a(n) = – Σ_{k=1}^{160} φ_k · a(n‑k),
   with a(0) = 1.  The sequence is 374‑periodic. */

#include <stdbool.h>

#define PERIOD 374
#define DEG_PH187 160
#define DEG_F 188
#define DEG_DEN 28

seq_t a(idx_t n)
{
    static seq_t period[PERIOD];
    static bool init = false;

    if (!init) {
        init = true;

        /* Coefficients for the division (x‑1)(x^187‑1) / (x^11‑1)(x^17‑1). */
        seq_t num[DEG_F + 1] = {0};
        seq_t den[DEG_DEN + 1] = {0};
        seq_t q[DEG_PH187 + 1] = {0};

        /* Numerator: (x-1)(x^187-1) = x^188 - x^187 - x + 1 */
        num[0] =  1;       /* +1 */
        num[1] = -1;       /* –1 */
        num[187] = -1;     /* –1 */
        num[188] =  1;     /* +1 */

        /* Denominator: (x^11-1)(x^17-1) = x^28 - x^17 - x^11 + 1 */
        den[0] =  1;       /* +1 */
        den[11] = -1;      /* –1 */
        den[17] = -1;      /* –1 */
        den[28] =  1;      /* +1 */

        /* Exact polynomial long division: num / den → q (degree 160). */
        for (int deg = DEG_F; deg >= DEG_DEN; --deg) {
            seq_t coef = num[deg];
            int shift = deg - DEG_DEN;

            q[shift] = coef;

            /* Subtract coef * den * x^shift from num. */
            num[shift]     -= coef * den[0];
            num[shift + 11]-= coef * den[11];
            num[shift + 17]-= coef * den[17];
            num[shift + 28]-= coef * den[28];
        }

        /* Φ_374(x) = Φ_187(‑x): flip sign of odd‑indexed coefficients. */
        seq_t phi[DEG_PH187 + 1];
        for (int k = 0; k <= DEG_PH187; ++k) {
            phi[k] = (k & 1) ? -(q[k]) : q[k];
        }

        /* Build the 374‑periodic sequence using the recurrence. */
        period[0] = 1;  /* a(0) = 1 */
        for (idx_t i = 1; i < PERIOD; ++i) {
            seq_t sum = 0;
            int m = (i <= DEG_PH187) ? (int)i : DEG_PH187;
            for (int k = 1; k <= m; ++k) {
                sum += phi[k] * period[i - (idx_t)k];
            }
            period[i] = -sum;
        }
    }

    return period[n % PERIOD];
}
