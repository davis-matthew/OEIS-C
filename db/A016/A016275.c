/*
 * A016275  Inverse of 2266th cyclotomic polynomial.
 * https://oeis.org/A016275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the inverse series of the
 *            cyclotomic polynomial Φ_2266(x). Φ_2266(x) has degree 1020, and
 *            its coefficients give a linear homogeneous recurrence with
 *            constant coefficients. Because all roots of Φ_2266 are 2266th
 *            roots of unity, the resulting sequence is periodic w...
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Internal polynomial representation */
typedef struct {
    seq_t *coef;   /* lowest degree first (coef[0] = constant term) */
    size_t len;    /* number of coefficients = degree + 1 */
} Poly;

/* ------------------------------------------- */
/* Basic polynomial arithmetic (naïve O(nm)).    */
static Poly poly_mul(const Poly *a, const Poly *b)
{
    Poly r;
    r.len = a->len + b->len - 1;
    r.coef = (seq_t *)calloc(r.len, sizeof(seq_t));
    for (size_t i = 0; i < a->len; ++i) {
        for (size_t j = 0; j < b->len; ++j)
            r.coef[i + j] += a->coef[i] * b->coef[j];
    }
    return r;
}

/* divides a monic divisor from a polynomial (exact division)   */
static Poly poly_div(const Poly *num, const Poly *den)
{
    size_t n = num->len, d = den->len;
    size_t qlen = n - d + 1;
    seq_t *q = (seq_t *)calloc(qlen, sizeof(seq_t));
    seq_t *tmp = (seq_t *)calloc(n, sizeof(seq_t));
    for (size_t i = 0; i < n; ++i) tmp[i] = num->coef[i];

    for (int i = (int)n - 1; i >= (int)d - 1; --i) {
        seq_t coeff = tmp[i];
        int qpos = i - (int)d + 1;
        q[qpos] = coeff;
        if (coeff == 0) continue;
        for (size_t j = 0; j < d; ++j)
            tmp[qpos + j] -= coeff * den->coef[j];
    }
    free(tmp);

    Poly r;
    r.len = qlen;
    r.coef = q;
    return r;
}

/* ------------------------------------------- */
/* Compute Φ_d for every divisor d of 2266.   */
/* The list of divisors of 2266 = 2*11*103.   */
static const uint64_t g_divs[8] = {1ULL, 2ULL, 11ULL, 22ULL,
                                   103ULL, 206ULL, 1133ULL, 2266ULL};

/* indexes g_divs: 0..7                                                    */
static size_t find_div_index(uint64_t d)
{
    for (size_t i = 0; i < 8; ++i)
        if (g_divs[i] == d) return i;
    return 8; /* should not happen                                            */
}

/* returns array of length 8 of Polys; caller frees coefficients   */
static Poly* compute_phi_divisors(void)
{
    Poly *phi = (Poly *)malloc(8 * sizeof(Poly));
    for (size_t idx = 0; idx < 8; ++idx) {
        uint64_t d = g_divs[idx];
        if (d == 1) {   /* Φ_1(x) = x-1 */
            phi[idx].len = 2;
            phi[idx].coef = (seq_t *)malloc(2 * sizeof(seq_t));
            phi[idx].coef[0] = -1;
            phi[idx].coef[1] =  1;
            continue;
        }

        /* numerator x^d - 1 */
        Poly numer;
        numer.len = d + 1;
        numer.coef = (seq_t *)calloc(d + 1, sizeof(seq_t));
        numer.coef[0] = -1;
        numer.coef[d] =  1;

        /* product of Φ_f for proper divisors f of d */
        Poly prod;
        prod.len = 1;
        prod.coef = (seq_t *)malloc(sizeof(seq_t));
        prod.coef[0] = 1;   /* monic 1 */

        for (size_t j = 0; j < idx; ++j) {   /* divisors in ascending order */
            if (d % g_divs[j] == 0) {
                Poly mul = poly_mul(&prod, &phi[j]);
                free(prod.coef);
                prod = mul;
            }
        }

        /* divide numerator by product */
        Poly quot = poly_div(&numer, &prod);
        free(numer.coef);
        free(prod.coef);

        phi[idx] = quot;
    }
    return phi;
}

/* ------------------------------------------- */
/* Main sequence function                      */
seq_t a(idx_t n)
{
    static bool computed = false;
    static seq_t period[2266];   /* periodic values                 */

    if (!computed) {
        /* compute Φ_2266 coefficients */
        Poly *phi_d = compute_phi_divisors();
        size_t idx_2266 = find_div_index(2266ULL);
        Poly phi2266 = phi_d[idx_2266];

        size_t deg = phi2266.len - 1;   /* degree 1020 for 2266 */

        /* recurrence coefficients: c_k = coeff of x^k (k≥1) in Φ_2266(x) */
        const seq_t *c = phi2266.coef;

        /* initial term */
        period[0] = 1;

        /* compute the 2266 period values */
        for (size_t i = 1; i < 2266; ++i) {
            seq_t sum = 0;
            size_t lim = (i < deg) ? i : deg;
            for (size_t k = 1; k <= lim; ++k)
                sum += c[k] * period[i - k];
            period[i] = -sum;
        }

        /* free temporary polynomials */
        for (size_t i = 0; i < 8; ++i)
            free(phi_d[i].coef);
        free(phi_d);

        computed = true;
    }

    return period[n % 2266];
}
