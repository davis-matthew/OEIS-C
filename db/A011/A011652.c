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
 * Validated: full, pass, 37 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Multiply a polynomial {!poly} (deg = *deg) by (x^d–1). */
static void multiply_by_xd_minus_one(seq_t **poly, size_t *deg, int d)
{
    size_t newdeg = *deg + (size_t)d;
    seq_t *newpoly = mem_new(seq_t, newdeg + 1);
    for (size_t i = 0; i <= *deg; ++i)
    {
        newpoly[i]          -= (*poly)[i];    /* multiply by –1 */
        newpoly[i + d]      += (*poly)[i];    /* multiply by x^d */
    }
    mem_free(*poly);
    *poly = newpoly;
    *deg = newdeg;
}

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
    seq_t *num = mem_new(seq_t, 1);
    num[0] = (seq_t)1;
    size_t num_deg = 0;
    int num_factors[] = { 2, 3, 19, 114 };
    for (int i = 0; i < 4; ++i)
        multiply_by_xd_minus_one(&num, &num_deg, num_factors[i]);

    /* -------- build denominator (x–1)(x^6–1)(x^38–1)(x^57–1) -------- */
    seq_t *den = mem_new(seq_t, 1);
    den[0] = (seq_t)1;
    size_t den_deg = 0;
    int den_factors[] = { 1, 6, 38, 57 };
    for (int i = 0; i < 4; ++i)
        multiply_by_xd_minus_one(&den, &den_deg, den_factors[i]);

    /* -------- division: numerator ÷ denominator -------- */
    size_t qlen = num_deg - den_deg + 1;            /* degree of Φ₁₁₄ = 36 */
    seq_t *phi = mem_new(seq_t, qlen);
    for (size_t i = 0; i < qlen; ++i)
        phi[i] = (seq_t)0;

    seq_t *num_mod = mem_new(seq_t, (num_deg + 1));
    for (size_t i = 0; i <= num_deg; ++i)
        num_mod[i] = num[i];
    mem_free(num);

    int nd = (int)den_deg;
    for (int i = (int)num_deg; i >= nd; --i)
    {
        seq_t coeff = num_mod[i];
        phi[i - nd] = coeff;
        for (size_t j = 0; j <= den_deg; ++j)
            num_mod[i - nd + j] -= coeff * den[j];
    }

    mem_free(num_mod);
    mem_free(den);

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
