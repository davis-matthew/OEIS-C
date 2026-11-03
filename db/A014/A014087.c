/*
 * A014087  Inverse of 78th cyclotomic polynomial.
 * https://oeis.org/A014087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series 1/Φ_78(x). It
 *            satisfies the recurrence a(n) = -∑_{i=1}^{24} φ_i · a(n-i)
 *            (n≥1), where (φ_0,φ_1,…,φ_24) are the coefficients of the 78‑th
 *            cyclotomic polynomial Φ_78(x). The sequence is periodic with
 *            period 78 (a(n+78)=a(n))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 97 terms through n = 825 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* compute the 25 coefficients of Φ_78(x) (degree 24).
   The coefficients are stored in coeff[0..24] (coeff[0] = 1).  */
static void compute_phi78_coeffs(seq_t coeff[25])
{
    /* sizes: numerator degree 96, denominator degree 72 */
    const int NUM_DEG = 96;
    const int DEN_DEG = 72;
    const int RES_DEG = NUM_DEG - DEN_DEG;          /* 24 */
    const int MAX_DEG = NUM_DEG;                    /* 96 */

    int32_t poly[MAX_DEG + 1];
    int32_t tmp[MAX_DEG + 1];
    int32_t num[MAX_DEG + 1];
    int32_t den[MAX_DEG + 1];
    int32_t temp[MAX_DEG + 1];
    int32_t q[RES_DEG + 1];

    /* ---- numerator (x^78-1)(x^13-1)(x^3-1)(x^2-1) ---- */
    memset(poly, 0, sizeof(poly));
    poly[0] = 1;
    int deg = 0;
    const uint8_t num_k[4] = {78, 13, 3, 2};
    for (int idx = 0; idx < 4; ++idx) {
        uint8_t k = num_k[idx];
        memset(tmp, 0, sizeof(tmp));
        for (int i = 0; i <= deg; ++i) {
            tmp[i]     -= poly[i];
            tmp[i + k] += poly[i];
        }
        memcpy(poly, tmp, sizeof(tmp));
        deg += k;
    }
    memcpy(num, poly, sizeof(num));

    /* ---- denominator (x^39-1)(x^26-1)(x^6-1)(x-1) ---- */
    memset(poly, 0, sizeof(poly));
    poly[0] = 1;
    deg = 0;
    const uint8_t den_k[4] = {39, 26, 6, 1};
    for (int idx = 0; idx < 4; ++idx) {
        uint8_t k = den_k[idx];
        memset(tmp, 0, sizeof(tmp));
        for (int i = 0; i <= deg; ++i) {
            tmp[i]     -= poly[i];
            tmp[i + k] += poly[i];
        }
        memcpy(poly, tmp, sizeof(tmp));
        deg += k;
    }
    memcpy(den, poly, sizeof(den));

    /* ---- exact division num / den ---- */
    memcpy(temp, num, sizeof(temp));
    memset(q, 0, sizeof(q));

    /* Perform long division from highest degree downwards */
    for (int i = NUM_DEG; i >= DEN_DEG; --i) {
        int32_t c = temp[i];
        if (c != 0) {
            int qidx = i - DEN_DEG;
            q[qidx] = c;
            for (int j = 0; j <= DEN_DEG; ++j) {
                temp[qidx + j] -= c * den[j];
            }
        }
    }

    /* Store the coefficients as seq_t */
    for (int i = 0; i <= RES_DEG; ++i) {
        coeff[i] = (seq_t)q[i];
    }
}

/* Entry point required by the test harness */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t coeff[25];   /* Φ_78 coefficients, φ_0..φ_24 */
    static seq_t period[78];  /* a(0)..a(77) */

    if (!ready) {
        /* compute Φ_78 coefficients once */
        compute_phi78_coeffs(coeff);

        /* generate the first 78 terms using the recurrence */
        period[0] = 1;
        for (int i = 1; i < 78; ++i) {
            seq_t s = 0;
            int limit = (i < 24) ? i : 24;
            for (int j = 1; j <= limit; ++j) {
                s += coeff[j] * period[i - j];
            }
            period[i] = -s;
        }
        ready = true;
    }
    return period[(size_t)(n % 78)];
}
