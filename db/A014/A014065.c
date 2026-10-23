/*
 * A014065  Inverse of 56th cyclotomic polynomial.
 * https://oeis.org/A014065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ_56(x); The sequence is periodic with period length 56
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t period[56];
    static int initialized = 0;
    if (!initialized) {
        /* Build the 56th cyclotomic polynomial Φ_56(x). */
        /* Φ_56(x) = (x^56 - 1)*(x^4 - 1) / [(x^28 - 1)*(x^8 - 1)] */
        seq_t num[61] = {0};
        seq_t den[37] = {0};
        /* numerator: (x^56 - 1)*(x^4 - 1) */
        seq_t tmp[61] = {0};
        num[0] = - (seq_t)1;
        num[56] = (seq_t)1;
        tmp[0] = - (seq_t)1;
        tmp[4] = (seq_t)1;
        seq_t num_mult[61] = {0};
        for (idx_t i = 0; i <= 56; ++i) {
            if (num[i] == 0) continue;
            for (idx_t j = 0; j <= 4; ++j) {
                if (tmp[j] == 0) continue;
                num_mult[i + j] += num[i] * tmp[j];
            }
        }
        /* denominator: (x^28 - 1)*(x^8 - 1) */
        seq_t den1[37] = {0};
        seq_t den2[37] = {0};
        den1[0] = - (seq_t)1;
        den1[28] = (seq_t)1;
        den2[0] = - (seq_t)1;
        den2[8] = (seq_t)1;
        seq_t den_mult[37] = {0};
        for (idx_t i = 0; i <= 28; ++i) {
            if (den1[i] == 0) continue;
            for (idx_t j = 0; j <= 8; ++j) {
                if (den2[j] == 0) continue;
                den_mult[i + j] += den1[i] * den2[j];
            }
        }
        /* Perform polynomial long division to obtain Φ_56(x) as a quotient. */
        const int DEG_N = 60;   /* degree of numerator */
        const int DEG_D = 36;   /* degree of denominator */
        const int DEG_PHI = 24; /* degree of Φ_56(x) */
        seq_t rem[61];
        for (int i = 0; i <= DEG_N; ++i)
            rem[i] = num_mult[i];
        seq_t phi[25] = {0};
        for (int i = DEG_N; i >= DEG_D; --i) {
            seq_t coeff = rem[i];
            if (coeff != 0) {
                int pos = i - DEG_D;   /* index in quotient */
                phi[pos] = coeff;      /* coefficient of Φ_56(x) */
                for (int j = 0; j <= DEG_D; ++j) {
                    rem[pos + j] -= coeff * den_mult[j];
                }
            }
        }
        /* Now compute the inverse power series of Φ_56(x) up to length 56. */
        seq_t series_local[56];
        series_local[0] = (seq_t)1;
        for (idx_t k = 1; k < 56; ++k) {
            seq_t sum = (seq_t)0;
            for (int i = 1; i <= DEG_PHI && i <= (int)k; ++i) {
                sum += phi[i] * series_local[k - i];
            }
            series_local[k] = -sum;
        }
        /* Copy the period into the static array. */
        for (int i = 0; i < 56; ++i)
            period[i] = series_local[i];
        initialized = 1;
    }
    return period[n % 56];
}
