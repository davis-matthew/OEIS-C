/*
 * A022496  Conjectured number of irreducible multiple zeta values of depth 8
 *          and weight 2n+22.
 * https://oeis.org/A022496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1 + 3*x + 7*x^2 + 8*x^3 + 13*x^4 +
 *            14*x^5 + 15*x^6 + 16*x^7 + 8*x^8 + 10*x^9 + 4*x^11 - 3*x^12 +
 *            x^13 - 2*x^14 + x^15 + x^17) divided by
 *            ((1-x)^2*(1-x^3)^2*(1-x^4)^2*(1-x^6)^2)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* numerator coefficients (index corresponds to power of x) */
static const int numer[18] = {
    1,   /* x^0  */
    3,   /* x^1  */
    7,   /* x^2  */
    8,   /* x^3  */
    13,  /* x^4  */
    14,  /* x^5  */
    15,  /* x^6  */
    16,  /* x^7  */
    8,   /* x^8  */
    10,  /* x^9  */
    0,   /* x^10 */
    4,   /* x^11 */
    -3,  /* x^12 */
    1,   /* x^13 */
    -2,  /* x^14 */
    1,   /* x^15 */
    0,   /* x^16 */
    1    /* x^17 */
};

/* denominator coefficients D(x) = (1-x)^2*(1-x^3)^2*(1-x^4)^2*(1-x^6)^2 */
/* coeff_d[i] corresponds to coefficient of x^i in D(x), used for i >= 1 */
static int coeff_d[29];
static bool coeff_d_init = false;

/* series 1/D(x)  =  Σ f[n] x^n  (f[0] = 1) */
static seq_t *f = NULL;
static size_t f_len = 0;

/* compute the coefficients of D(x) */
static void init_coeffs(void)
{
    if (coeff_d_init) return;

    const int ks[4] = {1, 3, 4, 6};
    int poly[29] = {0};
    int newpoly[29] = {0};

    poly[0] = 1;  /* start with 1 */

    for (int idx = 0; idx < 4; ++idx) {
        int k = ks[idx];
        for (int i = 0; i < 29; ++i) newpoly[i] = 0;
        for (int i = 0; i < 29; ++i) {
            if (poly[i] == 0) continue;
            /* (1 - 2x^k + x^{2k}) * poly */
            if (i <= 28)           newpoly[i]     += poly[i];
            if (i + k <= 28)       newpoly[i + k] += -2 * poly[i];
            if (i + 2 * k <= 28)   newpoly[i + 2*k] += poly[i];
        }
        for (int i = 0; i < 29; ++i) poly[i] = newpoly[i];
    }
    for (int i = 0; i < 29; ++i) coeff_d[i] = poly[i];
    coeff_d_init = true;
}

/* ensure the reciprocal series f[] is computed up to index n */
static void ensure_f(idx_t n)
{
    if (f_len > (size_t)n) return;

    size_t newlen = (size_t)n + 1;
    f = (seq_t *)realloc(f, newlen * sizeof(seq_t));

    /* zero new entries */
    for (size_t i = f_len; i < newlen; ++i) f[i] = 0;

    init_coeffs();

    if (f_len == 0) {
        f[0] = 1;
        f_len = 1;
    }

    for (idx_t idx = f_len; idx <= n; ++idx) {
        seq_t acc = 0;
        for (int i = 1; i <= 28; ++i) {
            if (idx >= (idx_t)i) {
                seq_t term = (seq_t)coeff_d[i] * f[idx - i];
                acc += term;
            }
        }
        f[idx] = -acc;
    }
    f_len = newlen;
}

/* OEIS A022496: Conjectured number of irreducible multiple zeta values of depth 8 and weight 2n+22 */
seq_t a(idx_t n)
{
    ensure_f(n);

    seq_t result = 0;
    for (int i = 0; i < 18; ++i) {
        if (n >= (idx_t)i) {
            seq_t term = (seq_t)numer[i] * f[n - i];
            result += term;
        }
    }
    return result;
}
