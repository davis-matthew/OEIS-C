/*
 * A011651  110th cyclotomic polynomial.
 * https://oeis.org/A011651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 40, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Φ₁₁₀(x) = ∏_{d|110} (x^{110/d} − 1)^{μ(d)}
    coefficients of Φ₁₁₀(x) as a(n) for n = 0,1,2,…  */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DEG 40      /* φ(110) = 40 */

/* coefficients for x^0 … x^MAX_DEG */
static seq_t coeffs[MAX_DEG + 1] = {0};
static bool coeffs_built = false;

/* multiply the current polynomial (old[]) by the factor
 * (x^m − 1)^μ where μ is +1 or −1; result is written to newc[] */
static void multiply_factor(idx_t m, int mu, seq_t *old, seq_t *newc)
{
    /* clear newc */
    for (idx_t i = 0; i <= MAX_DEG; ++i)
        newc[i] = 0;

    if (mu > 0) {                     /* multiply by (x^m − 1) */
        for (idx_t i = 0; i <= MAX_DEG; ++i) {
            seq_t val = old[i];
            if (!val)
                continue;
            /* term from x^i  */
            newc[i] -= val;
            /* term from x^{i+m}  */
            if (i + m <= MAX_DEG)
                newc[i + m] += val;
        }
    } else {                            /* multiply by (x^m − 1)^{-1}
                                           = 1 + x^m + x^{2m} + … */
        for (idx_t i = 0; i <= MAX_DEG; ++i) {
            seq_t val = old[i];
            if (!val)
                continue;
            /* add geometric series over multiples of m */
            for (idx_t pos = i; pos <= MAX_DEG; pos += m)
                newc[pos] += val;
        }
    }
}

/* build the coefficient array once */
static void compute_coeffs(void)
{
    static seq_t old[MAX_DEG + 1];
    static seq_t newc[MAX_DEG + 1];

    /* start with the constant polynomial 1 */
    for (idx_t i = 0; i <= MAX_DEG; ++i)
        old[i] = 0;
    old[0] = 1;

    /* divisors of 110 in increasing order */
    const idx_t divs[] = {1, 2, 5, 10, 11, 22, 55, 110};
    const int  mu[]   = { 1, -1, -1,  1, -1,  1,  1, -1 };
    const size_t divcnt = sizeof(divs) / sizeof(divs[0]);

    for (size_t d = 0; d < divcnt; ++d) {
        idx_t m = 110 / divs[d];      /* exponent in (x^m − 1) */
        multiply_factor(m, mu[d], old, newc);
        /* copy newc back to old for next iteration */
        for (idx_t i = 0; i <= MAX_DEG; ++i)
            old[i] = newc[i];
    }

    /* store final coefficients */
    for (idx_t i = 0; i <= MAX_DEG; ++i)
        coeffs[i] = old[i];

    coeffs_built = true;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!coeffs_built)
        compute_coeffs();

    if (n <= MAX_DEG)
        return coeffs[n];
    /* coefficients beyond the degree of Φ₁₀₁₀(x) are zero */
    return (seq_t)0;
}
