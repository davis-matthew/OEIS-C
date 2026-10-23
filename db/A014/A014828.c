/*
 * A014828  Inverse of 819th cyclotomic polynomial.
 * https://oeis.org/A014828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ₁₈₀₉(x), where Φ₁₈₀₉ is the 819th cyclotomic polynomial. The
 *            sequence is periodic with period 819
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t coeff[819];
static bool init_done = false;

static void init_coeffs(void)
{
    /* ----- 1. compute 1/Φ₈¹⁹ using product of factors ----- */
    size_t i, n;
    for (i = 0; i < 819; ++i) coeff[i] = 0;
    coeff[0] = 1;   /* start with 1 */

    /* Factors with μ = +1 : (1 - x^d)^{-1} = 1 + x^d + x^{2d} + ...   */
    const unsigned long d_plus[] = {9, 21, 39, 819};
    const unsigned long d_plus_len = sizeof(d_plus) / sizeof(d_plus[0]);

    for (unsigned long j = 0; j < d_plus_len; ++j)
    {
        unsigned long d = d_plus[j];
        if (d == 819) continue;          /* contributes only the constant term for n<819 */
        for (n = d; n < 819; ++n)
            coeff[n] += coeff[n - d];
    }

    /* Factors with μ = -1 : (1 - x^d) = 1 - x^d */
    const unsigned long d_minus[] = {3, 63, 117, 273};
    const unsigned long d_minus_len = sizeof(d_minus) / sizeof(d_minus[0]);

    for (unsigned long j = 0; j < d_minus_len; ++j)
    {
        unsigned long d = d_minus[j];
        seq_t tmp[819];
        for (i = 0; i < 819; ++i) tmp[i] = 0;
        for (i = 0; i < 819; ++i)
        {
            tmp[i] += coeff[i];                  /* multiply by 1 */
            if (i + d < 819) tmp[i + d] -= coeff[i]; /* subtract the x^d piece */
        }
        for (i = 0; i < 819; ++i) coeff[i] = tmp[i];
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done) init_coeffs();
    return coeff[(unsigned long)(n % 819)];
}
