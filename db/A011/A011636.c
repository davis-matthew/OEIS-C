/*
 * A011636  45th cyclotomic polynomial.
 * https://oeis.org/A011636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 45th cyclotomic polynomial
 *            Φ_45(x). The polynomial is Φ_45(x) = (x^45-1)(x^3-1) /
 *            ((x^9-1)(x^15-1))
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static void poly_mul(const seq_t *a, size_t na, const seq_t *b, size_t nb,
                     seq_t *res)
{
    size_t len = na + nb - 1;
    seq_t zero = (seq_t)0;
    for (size_t i = 0; i < len; ++i) res[i] = zero;
    for (size_t i = 0; i < na; ++i)
        for (size_t j = 0; j < nb; ++j)
            res[i + j] += a[i] * b[j];
}

static void poly_div_monic(const seq_t *num, size_t nlen,
                           const seq_t *den, size_t dlen,
                           seq_t *quot)
{
    seq_t rem[50];            /* sufficient for numerator (degree 48) */
    seq_t zero = (seq_t)0;
    for (size_t i = 0; i < 50; ++i) rem[i] = zero;
    for (size_t i = 0; i < nlen; ++i) rem[i] = num[i];

    size_t rem_deg = nlen - 1;
    size_t den_deg = dlen - 1;

    seq_t q[25];              /* quotient degree will be 24 */
    for (size_t i = 0; i < 25; ++i) q[i] = zero;

    while (rem_deg >= den_deg) {
        seq_t factor = rem[rem_deg];
        size_t shift = rem_deg - den_deg;
        q[shift] = factor;
        for (size_t i = 0; i < dlen; ++i)
            rem[i + shift] -= factor * den[i];
        while (rem_deg > 0 && rem[rem_deg] == zero) --rem_deg;
    }
    for (size_t i = 0; i < 25; ++i) quot[i] = q[i];
}

static void compute_phi45(seq_t *coeffs)
{
    /* Build polynomials for x^k - 1 */
    seq_t x45[46] = {0}, x3[4] = {0}, x9[10] = {0}, x15[16] = {0};
    seq_t zero = (seq_t)0;
    /* x^45 - 1 */
    x45[0] = - ((seq_t)1);
    x45[45] = (seq_t)1;
    /* x^3 - 1 */
    x3[0] = - ((seq_t)1);
    x3[3] = (seq_t)1;
    /* x^9 - 1 */
    x9[0] = - ((seq_t)1);
    x9[9] = (seq_t)1;
    /* x^15 - 1 */
    x15[0] = - ((seq_t)1);
    x15[15] = (seq_t)1;

    /* Numerator: (x^45-1)*(x^3-1) */
    seq_t num[49]; /* degree 48 -> 49 terms */
    poly_mul(x45, 46, x3, 4, num);
    /* Denominator: (x^9-1)*(x^15-1) */
    seq_t den[25]; /* degree 24 -> 25 terms */
    poly_mul(x9, 10, x15, 16, den);

    /* Quotient: Φ_45(x) */
    seq_t quot[25];
    poly_div_monic(num, 49, den, 25, quot);

    for (size_t i = 0; i < 25; ++i) coeffs[i] = quot[i];
}

seq_t a(idx_t n)
{
    static seq_t coeffs[25];
    static bool initialized = false;
    if (!initialized) {
        compute_phi45(coeffs);
        initialized = true;
    }
    if (n < 25) return coeffs[(size_t)n];
    return (seq_t)0;
}
