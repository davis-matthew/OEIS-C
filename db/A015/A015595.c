/*
 * A015595  Inverse of 1586th cyclotomic polynomial.
 * https://oeis.org/A015595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1 / Φ_{1586}(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_DEG 1662          /* highest degree appearing in N or D */
#define MAX_N    1586          /* period length */
#define PHI_DEG  720

/* multiply polynomial `poly` of current degree *deg by (x^exp - 1) */
static void mul_factor(seq_t *poly, int *deg, int exp)
{
    static seq_t tmp[MAX_DEG + 1];
    int new_deg = *deg + exp;
    for (int i = 0; i <= new_deg; ++i) tmp[i] = 0;
    for (int i = 0; i <= *deg; ++i) {
        tmp[i + exp] += poly[i];
        tmp[i]        -= poly[i];
    }
    for (int i = 0; i <= new_deg; ++i) poly[i] = tmp[i];
    *deg = new_deg;
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t period[MAX_N];
    static seq_t poly_N[MAX_DEG + 1];
    static seq_t poly_D[MAX_DEG + 1];
    static seq_t phi[PHI_DEG + 1];

    if (!initialized) {
        /* ---------------- compute Φ_{1586}(x) via numerator/denominator ---------------- */
        int num_deg = 0, den_deg = 0;
        for (int i = 0; i <= MAX_DEG; ++i) { poly_N[i] = 0; poly_D[i] = 0; }
        poly_N[0] = 1; poly_D[0] = 1;

        /* numerator : (x^1586-1)(x^61-1)(x^13-1)(x^2-1) */
        mul_factor(poly_N, &num_deg, 1586);
        mul_factor(poly_N, &num_deg, 61);
        mul_factor(poly_N, &num_deg, 13);
        mul_factor(poly_N, &num_deg, 2);

        /* denominator : (x^793-1)(x^122-1)(x^26-1)(x-1) */
        mul_factor(poly_D, &den_deg, 793);
        mul_factor(poly_D, &den_deg, 122);
        mul_factor(poly_D, &den_deg, 26);
        mul_factor(poly_D, &den_deg, 1);

        /* exact division N / D  →  Φ_{1586}(x)  (degree 720) */
        for (int i = 0; i <= PHI_DEG; ++i) phi[i] = 0;
        int deg_N = num_deg;          /* 1662 */
        int deg_D = den_deg;          /* 942 */

        for (int i = deg_N; i >= deg_D; --i) {
            seq_t coef = poly_N[i];
            if (coef != 0) {
                int pos = i - deg_D;
                phi[pos] = coef;
                for (int j = 0; j <= deg_D; ++j) {
                    poly_N[pos + j] -= coef * poly_D[j];
                }
            }
        }

        /* ---------------- compute the period of 1/Φ_{1586}(x) ---------------- */
        period[0] = 1;   /* a(0) = 1 */
        for (int m = 1; m < MAX_N; ++m) {
            seq_t sum = 0;
            int max_i = m <= PHI_DEG ? m : PHI_DEG;
            for (int i = 1; i <= max_i; ++i) {
                sum += phi[i] * period[m - i];
            }
            period[m] = -sum;
        }

        initialized = 1;
    }

    idx_t mod = n % (idx_t)MAX_N;
    return period[(int)mod];
}
