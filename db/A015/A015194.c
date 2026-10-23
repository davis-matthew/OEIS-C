/*
 * A015194  Inverse of 1185th cyclotomic polynomial.
 * https://oeis.org/A015194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ₁₁₈₅(x), which is periodic with period 1185. The values are
 *            obtained by multiplying the factor series (xᵈ−1)^{−μ(n/d)} for
 *            all divisors d of 1185
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* internal cache of the 1185 periodic coefficients */
static seq_t period_coeff[1185];
static bool cache_built = false;

/* build the coefficient array once */
static void build_cache(void)
{
    const idx_t N = 1185;                         /* period length */
    const idx_t divs[] = {1, 3, 5, 15, 79, 237, 395, 1185};
    const int    mu[]   = {-1, 1, 1, -1, 1, -1, -1, 1};  /* μ(1185/d) */
    const size_t nd = sizeof(divs) / sizeof(divs[0]);

    seq_t res[1185]   = {0};
    seq_t new_res[1185];
    res[0] = 1;                                   /* identity element */

    for (size_t t = 0; t < nd; ++t) {
        idx_t d = divs[t];
        int   m = mu[t];

        if (m == 1) {      /* factor = -(1 + xᵈ + x²ᵈ + …) */
            for (idx_t i = 0; i < N; ++i) {
                seq_t sum = 0;
                for (idx_t k = 0; k * d <= i; ++k)
                    sum += res[i - k * d];
                new_res[i] = -sum;
            }
        } else {           /* factor = -1 + xᵈ */
            for (idx_t i = 0; i < N; ++i) {
                seq_t val = -res[i];
                if (i >= d)
                    val += res[i - d];
                new_res[i] = val;
            }
        }
        /* copy new_res back to res for next multiplication */
        for (idx_t i = 0; i < N; ++i)
            res[i] = new_res[i];
    }

    /* store the period in the static cache */
    for (idx_t i = 0; i < N; ++i)
        period_coeff[i] = res[i];
    cache_built = true;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (!cache_built)
        build_cache();
    return period_coeff[n % 1185];
}
