/*
 * A015256  Inverse of 1247th cyclotomic polynomial.
 * https://oeis.org/A015256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of 1 /
 *            Φ_1247(x). The sequence is periodic with period 1247
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define PERIOD 1247
#define NUM_DEG 1248         /* degree of (x^1248 – x^1247 – x + 1)          */
#define DEN_DEG 72          /* degree of (x^72 – x^43 – x^29 + 1)           */
#define QUO_DEG (NUM_DEG - DEN_DEG)   /* degree of Φ_1247(x) = 1176 */

/* Computes the n'th term of A015256. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t period[PERIOD];
    static seq_t phi[QUO_DEG + 1];
    static bool initialized = false;

    if (!initialized) {
        initialized = true;

        /* ---------- build numerator (x^1248 – x^1247 – x + 1) ---------- */
        seq_t numer[NUM_DEG + 1] = {0};
        numer[0]   = 1;     /* +1 */
        numer[1]   = -1;    /* –x */
        numer[1247] = -1;   /* –x^1247 */
        numer[1248] = 1;    /* +x^1248 */

        /* ---------- build denominator (x^72 – x^43 – x^29 + 1) ---------- */
        seq_t denom[DEN_DEG + 1] = {0};
        denom[0]   = 1;     /* +1 */
        denom[29]  = -1;    /* –x^29 */
        denom[43]  = -1;    /* –x^43 */
        denom[72]  = 1;     /* +x^72 */

        /* ---------- long division numer / denom to obtain Φ_1247(x) ----- */
        seq_t tmp[NUM_DEG + 1];
        for (int i = 0; i <= NUM_DEG; ++i) tmp[i] = numer[i];
        for (int i = NUM_DEG; i >= DEN_DEG; --i) {
            seq_t q = tmp[i];
            int pos = i - DEN_DEG;
            phi[pos] = q;
            if (q != 0) {
                for (int j = 0; j <= DEN_DEG; ++j) {
                    tmp[pos + j] -= q * denom[j];
                }
            }
        }

        /* ---------- compute the periodic sequence (recurrence) ------- */
        period[0] = 1;
        for (int idx = 1; idx < PERIOD; ++idx) {
            seq_t sum = 0;
            int maxk = (idx <= QUO_DEG) ? idx : QUO_DEG;
            for (int k = 1; k <= maxk; ++k) {
                sum += phi[k] * period[idx - k];
            }
            period[idx] = -sum;
        }
    }

    return period[(int)(n % PERIOD)];
}
