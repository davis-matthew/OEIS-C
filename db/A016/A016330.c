/*
 * A016330  130th cyclotomic polynomial.
 * https://oeis.org/A016330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 130th cyclotomic polynomial
 *            Φ₁₃₀(x)
 * Range:     i8 n <= 48, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* requires seq_t and idx_t to be defined elsewhere */
#include <stddef.h>   /* for size_t */

/* Maximum degrees after multiplying all factors */
#define NUM_DEG_MAX   150   /* 130 + 13 + 5 + 2  */
#define DENOM_DEG_MAX 102   /* 65  + 26 +10 + 1 */

/* Global coefficient array (degree φ(130) = 48) */
static seq_t coeff[49];
static int coeff_computed = 0;

/* Helper to multiply a polynomial by (x^k – 1) in place.
   poly[0..deg] is the current polynomial.  After the call
   poly[0..deg+k] contains the updated coefficients and *deg
   is set to deg + k.  All calculations use a 64‑bit intermediate
   to avoid overflow on small seq_t types. */
static void multiply_by_factor(seq_t poly[NUM_DEG_MAX + 1], size_t *deg, size_t k)
{
    size_t old_deg = *deg;
    size_t new_deg = old_deg + k;
    seq_t tmp[NUM_DEG_MAX + 1] = {0};

    for (size_t i = 0; i <= old_deg; ++i) {
        /* –poly[i] */
        tmp[i] = (seq_t)((long long)tmp[i] - (long long)poly[i]);
        /* +x^k*poly[i] */
        tmp[i + k] = (seq_t)((long long)tmp[i + k] + (long long)poly[i]);
    }
    for (size_t i = 0; i <= new_deg; ++i) {
        poly[i] = tmp[i];
    }
    *deg = new_deg;
}

/* Build all coefficients of Φ₁₃₀(x) on first call. */
static void build_polynomial(void)
{
    /* Numerical and denominator polynomials */
    static seq_t num[NUM_DEG_MAX + 1]   = {0};
    static seq_t denom[DENOM_DEG_MAX + 1] = {0};
    size_t deg_num   = 0;
    size_t deg_denom = 0;

    /* Start with the constant polynomial 1 */
    num[0] = 1;
    denom[0] = 1;

    /* Divisors of 130 and their Möbius values */
    const idx_t divisors[8] = {1, 2, 5, 10, 13, 26, 65, 130};
    const int    mu_vals[8]  = { 1, -1, -1,  1, -1,  1,  1, -1 };

    for (size_t i = 0; i < 8; ++i) {
        idx_t   d = divisors[i];
        size_t  k = 130 / d;          /* exponent of x in the factor */
        if (mu_vals[i] == 1) {
            multiply_by_factor(num, &deg_num, k);
        } else { /* mu = -1 */
            multiply_by_factor(denom, &deg_denom, k);
        }
    }

    /* Perform exact long division num / denom.
       denom is monic, so division is straightforward. */
    static seq_t d_poly[DENOM_DEG_MAX + 1];
    for (size_t i = 0; i <= deg_denom; ++i) d_poly[i] = denom[i];

    static seq_t q_poly[NUM_DEG_MAX + 1] = {0};
    size_t q_deg = 0;

    while (deg_num >= deg_denom) {
        seq_t factor = num[deg_num];      /* leading coefficient of numerator */
        size_t shift = deg_num - deg_denom;

        if (shift > q_deg) q_deg = shift;
        q_poly[shift] = factor;

        /* subtract factor * x^shift * d_poly from num */
        for (size_t j = 0; j <= deg_denom; ++j) {
            long long prod = (long long)factor * (long long)d_poly[j];
            num[j + shift] = (seq_t)((long long)num[j + shift] - prod);
        }

        /* reduce degree if the top coefficient became zero */
        while (deg_num > 0 && num[deg_num] == 0) --deg_num;
    }

    /* Copy the resulting coefficients (degree 48) into the global table. */
    for (size_t n = 0; n <= 48; ++n) {
        if (n <= q_deg) coeff[n] = q_poly[n];
        else            coeff[n] = 0;
    }

    coeff_computed = 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (!coeff_computed) build_polynomial();

    if (n < 49)        /* 0‑based indices for the 49 coefficients */
        return coeff[n];
    else
        return 0;
}
