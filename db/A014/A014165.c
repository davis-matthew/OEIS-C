/*
 * A014165  Inverse of 156th cyclotomic polynomial.
 * https://oeis.org/A014165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series 1/Φ_156(x),
 *            which is periodic with period 156. Φ_156(x) is the 156th
 *            cyclotomic polynomial. The implementation constructs Φ_156(x)
 *            from its product representation and then inverts it term‑by‑term
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD   156        /* length of the periodic sequence   */
#define PHI_DEG  48         /* degree of Φ_156(x)                */
#define MAX_DEG  200        /* buffer size for intermediate polys */

static void poly_mul_shift(seq_t *poly, size_t *deg, size_t k)
{
    seq_t tmp[MAX_DEG + 1] = {0};
    for (size_t i = 0; i <= *deg; ++i) {
        tmp[i]     -= poly[i];   /* subtract the original coeff          */
        tmp[i + k] += poly[i];   /* add the shifted coeff                */
    }
    *deg += k;
    for (size_t i = 0; i <= *deg; ++i)
        poly[i] = tmp[i];
}

/* Compute the 156th cyclotomic polynomial Φ_156(x);
   the result is stored in poly[0..*deg]. */
static void compute_phi156(seq_t *poly, size_t *deg)
{
    /* The Möbius‑sign table for the divisors of 156. */
    const struct { idx_t d; int mu; } divs[] = {
        {1,  1}, {2, -1}, {3, -1},
        {6,  1}, {13, -1}, {26, 1},
        {39, 1}, {78, -1}
    };
    const size_t nd = sizeof(divs) / sizeof(divs[0]);

    /* Separate numerator and denominator to avoid intermediate
       non‑integral polynomials. */
    seq_t num[MAX_DEG + 1] = {0};
    seq_t den[MAX_DEG + 1] = {0};
    size_t num_deg = 0, den_deg = 0;
    num[0] = 1;
    den[0] = 1;

    for (size_t i = 0; i < nd; ++i) {
        idx_t d  = divs[i].d;
        int   mu = divs[i].mu;
        size_t k = (size_t)(156U / d);

        if (mu == 1)
            poly_mul_shift(num, &num_deg, k);
        else
            poly_mul_shift(den, &den_deg, k);
    }

    /* Integer division : Φ_156 = num / den. */
    size_t res_deg = num_deg - den_deg;
    seq_t res[MAX_DEG + 1] = {0};
    seq_t work[MAX_DEG + 1];

    for (size_t i = 0; i <= num_deg; ++i)
        work[i] = num[i];

    for (int i = (int)num_deg; i >= (int)den_deg; --i) {
        seq_t coeff = work[i];
        res[i - den_deg] = coeff;
        for (size_t j = 0; j <= den_deg; ++j)
            work[i - j] -= coeff * den[den_deg - j];
    }

    *deg = res_deg;
    for (size_t i = 0; i <= res_deg; ++i)
        poly[i] = res[i];
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[PERIOD] = {0};
    static seq_t phi[PHI_DEG + 1] = {0};
    static size_t phi_deg = 0;

    if (!initialized) {
        /* 1. Compute Φ_156(x). */
        compute_phi156(phi, &phi_deg);

        /* 2. Invert the power series 1/Φ_156(x) term‑by‑term. */
        period[0] = 1;
        for (size_t i = 1; i < PERIOD; ++i) {
            seq_t sum = 0;
            size_t maxk = phi_deg;
            if (i < maxk) maxk = i;
            for (size_t k = 1; k <= maxk; ++k)
                sum += phi[k] * period[i - k];
            period[i] = -sum;
        }

        initialized = true;
    }

    return period[(size_t)(n % PERIOD)];
}
