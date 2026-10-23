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
 * Helpers:   divide_polynomials, multiply_by_xp_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*               Φ_182(x) = ∏_{d|182} (x^d − 1)^{μ(182/d)} */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divide_polynomials.h"
#include "../utils/multiply_by_xp_minus_one.h"

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
    seq_t *num = (seq_t *)calloc(n + 1, sizeof(seq_t));
    num[0] = (seq_t)-1;
    num[n] = (seq_t)1;
    size_t num_deg = n;

    for (size_t i = 0; i < num_primes; ++i) {
        multiply_by_xp_minus_one(&num, &num_deg, primes[i]);
    }

    /* ------------------- build denominator ------------------- */
    /* Start with (x − 1) */
    seq_t *den = (seq_t *)calloc(2, sizeof(seq_t));
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
    free(num);
    free(den);

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
