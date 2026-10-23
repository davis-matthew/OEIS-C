/*
 * A011652  114th cyclotomic polynomial.
 * https://oeis.org/A011652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in Φ₁₁₄(x) where Φ₁₁₄(x) =
 *            (x²–1)(x³–1)(x¹⁹–1)(x¹¹⁴–1) ÷ (x–1)(x⁶–1)(x³⁸–1)(x⁵⁷–1)
 * Range:     i8 n <= 36, i16 n <= 36, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   multiply_by_xd_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/multiply_by_xd_minus_one.h"

static seq_t *phi114_tbl = NULL;   /* coefficients of Φ₁₁₄(x) */
static size_t deg114_tbl = 0;      /* degree of Φ₁₁₄(x) = 36 */
static bool phi114_init = false;   /* flag indicating that φ₁₁₄ has been computed */

/* Build the coefficient array of Φ₁₁₄(x) and store it in the static
   table.  The construction follows the product/quotient definition
   above.  All intermediate polynomials are allocated on the heap
   and freed after use. */
static void compute_phi114(void)
{
    /* -------- build numerator (x^2–1)(x^3–1)(x^19–1)(x^114–1) -------- */
    seq_t *num = (seq_t*)malloc(sizeof(seq_t));
    num[0] = (seq_t)1;
    size_t num_deg = 0;
    int num_factors[] = { 2, 3, 19, 114 };
    for (int i = 0; i < 4; ++i)
        multiply_by_xd_minus_one(&num, &num_deg, num_factors[i]);

    /* -------- build denominator (x–1)(x^6–1)(x^38–1)(x^57–1) -------- */
    seq_t *den = (seq_t*)malloc(sizeof(seq_t));
    den[0] = (seq_t)1;
    size_t den_deg = 0;
    int den_factors[] = { 1, 6, 38, 57 };
    for (int i = 0; i < 4; ++i)
        multiply_by_xd_minus_one(&den, &den_deg, den_factors[i]);

    /* -------- division: numerator ÷ denominator -------- */
    size_t qlen = num_deg - den_deg + 1;            /* degree of Φ₁₁₄ = 36 */
    seq_t *phi = (seq_t*)malloc(qlen * sizeof(seq_t));
    for (size_t i = 0; i < qlen; ++i)
        phi[i] = (seq_t)0;

    seq_t *num_mod = (seq_t*)malloc((num_deg + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= num_deg; ++i)
        num_mod[i] = num[i];
    free(num);

    int nd = (int)den_deg;
    for (int i = (int)num_deg; i >= nd; --i)
    {
        seq_t coeff = num_mod[i];
        phi[i - nd] = coeff;
        for (size_t j = 0; j <= den_deg; ++j)
            num_mod[i - nd + j] -= coeff * den[j];
    }

    free(num_mod);
    free(den);

    phi114_tbl  = phi;
    deg114_tbl  = num_deg - den_deg;
    phi114_init = true;
}

/* The OEIS function a(n).  It returns the n‑th coefficient of Φ₁₁₄(x). */
seq_t a(idx_t n)
{
    if (!phi114_init)
        compute_phi114();

    if (n <= deg114_tbl)
        return phi114_tbl[(size_t)n];
    else
        return (seq_t)0;
}
