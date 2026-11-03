/*
 * A016348  182nd cyclotomic polynomial.
 * https://oeis.org/A016348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 182nd cyclotomic polynomial
 *            Φ_182(x)
 * Range:     i8 n <= 72, i16 n <= 72, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Validated: full, pass, 73 terms through n = 72 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*               Φ_182(x) = ∏_{d|182} (x^d − 1)^{μ(182/d)} */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Exact polynomial division:  result = numerator / denominator.
 * Both arrays are preserved; result is stored in a newly allocated array.
 * The leading coefficient of the denominator is assumed to be 1. */
static void divide_polynomials(const seq_t *numer, size_t numer_deg,
                               const seq_t *denom, size_t denom_deg,
                               seq_t **quotient, size_t *quotient_deg)
{
    size_t qlen = numer_deg - denom_deg + 1;
    seq_t *q = mem_new(seq_t, qlen);
    seq_t *temp = mem_new(seq_t, (numer_deg + 1));
    for (size_t i = 0; i <= numer_deg; ++i) temp[i] = numer[i];

    for (long long i = numer_deg; i >= (long long)denom_deg; --i) {
        /* coefficient of x^(i - denom_deg) in the quotient */
        seq_t c = temp[i];           /* denom leading coeff is 1 */
        size_t pos = (size_t)(i - denom_deg);
        q[pos] = c;
        /* subtract c * denom from the current portion of temp */
        for (size_t j = 0; j <= denom_deg; ++j) {
            temp[pos + j] -= c * denom[denom_deg - j];
        }
    }
    mem_free(temp);
    *quotient     = q;
    *quotient_deg = qlen - 1;
}

/* Multiply a polynomial by (x^p − 1) and replace it.
 * The caller owns and must free the old array. */
static void multiply_by_xp_minus_one(seq_t **poly, size_t *deg, size_t p)
{
    size_t old_deg = *deg;
    size_t new_deg = old_deg + p;
    seq_t *new_poly = mem_new(seq_t, new_deg + 1);
    for (size_t i = 0; i <= old_deg; ++i) {
        /* (x^p − 1) * poly  =  poly shifted by p minus poly itself */
        new_poly[i]      -= (*poly)[i];
        new_poly[i + p] += (*poly)[i];
    }
    mem_free(*poly);
    *poly = new_poly;
    *deg  = new_deg;
}

/* Static storage for the coefficients and degree of Φ_182(x). */
static bool        initialized = false;
static seq_t      *coeffs      = NULL;   /* pointer to coefficient array */
static size_t       deg          = 0;      /* degree of Φ_182 */

/* Build the coefficients of Φ_182(x) once. */
static void init_phi182(void)
{
    /* Primitive data for the construction. */
    const uint64_t n           = 182ULL;
    const size_t   num_primes  = 3;
    const size_t   primes[3]   = {2, 7, 13};
    const size_t   pair_count  = 3;
    const size_t   pair_prod[3]= {2 * 7, 2 * 13, 7 * 13};

    /* ------------------- build numerator ------------------- */
    /* Start with x^n − 1 */
    seq_t *num = mem_new(seq_t, n + 1);
    num[0] = (seq_t)-1;
    num[n] = (seq_t)1;
    size_t num_deg = n;

    for (size_t i = 0; i < num_primes; ++i) {
        multiply_by_xp_minus_one(&num, &num_deg, primes[i]);
    }

    /* ------------------- build denominator ------------------- */
    /* Start with (x − 1) */
    seq_t *den = mem_new(seq_t, 2);
    den[0] = (seq_t)-1;
    den[1] = (seq_t)1;
    size_t den_deg = 1;

    for (size_t i = 0; i < pair_count; ++i) {
        multiply_by_xp_minus_one(&den, &den_deg, pair_prod[i]);
    }

    /* ------------------- divide to get Φ_182 ------------------- */
    seq_t *res;
    size_t res_deg;
    divide_polynomials(num, num_deg, den, den_deg, &res, &res_deg);

    /* Store the result */
    coeffs = res;
    deg    = res_deg;

    /* Clean up numerator and denominator */
    mem_free(num);
    mem_free(den);

    initialized = true;
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    if (!initialized) init_phi182();

    if (n <= (idx_t)deg) {
        return coeffs[(size_t)n];
    }
    return (seq_t)0;
}
