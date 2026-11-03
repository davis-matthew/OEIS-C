/*
 * A016390  357th cyclotomic polynomial.
 * https://oeis.org/A016390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 357th cyclotomic polynomial
 *            Φ_357(x)
 * Range:     i8 n <= 192, i16 n <= 192, i32 n <= 192, i64 n <= 192,
 *            i128 n <= 192, big any n
 * Validated: full, pass, 193 terms through n = 192 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Polynomial structure: degree and array of coefficients (coef[0] is constant term). */
struct poly_s {
    seq_t *coef;   /* coef[i] is coefficient of x^i */
    size_t deg;    /* highest exponent with non‑zero coefficient */
};

/* Multiply two polynomials: c = a * b */
static struct poly_s *poly_mul(const struct poly_s *a, const struct poly_s *b)
{
    size_t new_deg = a->deg + b->deg;
    struct poly_s *c = (struct poly_s *)malloc(sizeof *c);
    c->deg = new_deg;
    c->coef = mem_new(seq_t, new_deg + 1);

    for (size_t i = 0; i <= a->deg; ++i) {
        seq_t ai = a->coef[i];
        if (ai == 0) continue;
        for (size_t j = 0; j <= b->deg; ++j) {
            seq_t bj = b->coef[j];
            if (bj == 0) continue;
            c->coef[i + j] += ai * bj;
        }
    }
    return c;
}

/* Divide polynomial num by div (div is monic and divides num exactly). */
static struct poly_s *poly_div(const struct poly_s *num, const struct poly_s *div)
{
    size_t deg_num = num->deg;
    size_t deg_div = div->deg;

    if (deg_num < deg_div) {
        /* Result is zero polynomial. */
        struct poly_s *res = (struct poly_s *)malloc(sizeof *res);
        res->deg = 0;
        res->coef = mem_new(seq_t, 1);
        return res;
    }

    size_t deg_quo = deg_num - deg_div;
    struct poly_s *quo = (struct poly_s *)malloc(sizeof *quo);
    quo->deg = deg_quo;
    quo->coef = mem_new(seq_t, deg_quo + 1);

    /* Work copy of numerator coefficients. */
    seq_t *work = mem_new(seq_t, deg_num + 1);
    for (size_t i = 0; i <= deg_num; ++i) work[i] = num->coef[i];

    for (int i = (int)deg_num; i >= (int)deg_div; --i) {
        seq_t coeff = work[i];
        if (coeff == 0) continue;
        size_t qpos = i - deg_div;
        quo->coef[qpos] = coeff;
        for (size_t j = 0; j <= deg_div; ++j) {
            work[qpos + j] -= coeff * div->coef[j];
        }
    }

    mem_free(work);
    return quo;
}

/* Create polynomial (x^d - 1). */
static struct poly_s *poly_xpow_minus1(idx_t d)
{
    struct poly_s *p = (struct poly_s *)malloc(sizeof *p);
    p->deg = (size_t)d;
    p->coef = mem_new(seq_t, d + 1);
    p->coef[0] = -1;
    p->coef[d] = 1;
    return p;
}

/* Global cache for Φ_357 coefficients. */
static seq_t coeff357[193];   /* 0 .. 192 */
static bool initialized = false;

/* Compute Φ_357 once and store its coefficients. */
static void compute_coeff357(void)
{
    /* Divisors of 357 in increasing order. */
    const size_t divs[8] = {1, 3, 7, 17, 21, 51, 119, 357};
    const size_t D = 8;

    struct poly_s *phis[D];

    for (size_t idx = 0; idx < D; ++idx) {
        idx_t d = divs[idx];
        if (idx == 0) {
            /* Φ₁(x) = x - 1 */
            phis[0] = (struct poly_s *)malloc(sizeof *phis[0]);
            phis[0]->deg = 1;
            phis[0]->coef = mem_new(seq_t, 2);
            phis[0]->coef[0] = -1;
            phis[0]->coef[1] = 1;
            continue;
        }

        /* Product of Φ_e for proper divisors e of d. */
        struct poly_s *prod = (struct poly_s *)malloc(sizeof *prod);
        prod->deg = 0;
        prod->coef = mem_new(seq_t, 1);
        prod->coef[0] = 1;   /* polynomial 1 */

        for (size_t j = 0; j < idx; ++j) {
            if (d % divs[j] == 0) {
                struct poly_s *tmp = poly_mul(prod, phis[j]);
                mem_free(prod->coef);
                free(prod);
                prod = tmp;
            }
        }

        /* Numerator (x^d - 1). */
        struct poly_s *num = poly_xpow_minus1(d);

        /* Φ_d = num / prod */
        struct poly_s *phi = poly_div(num, prod);

        /* Store Φ_d. */
        phis[idx] = phi;

        /* Free temporaries. */
        mem_free(num->coef);
        free(num);
        mem_free(prod->coef);
        free(prod);
    }

    /* Copy Φ_357 coefficients into coeff357 array. */
    struct poly_s *phi357 = phis[D-1];   /* last divisor is 357 */
    for (size_t i = 0; i <= phi357->deg; ++i)
        coeff357[i] = phi357->coef[i];

    /* Free all allocated polynomials. */
    for (size_t i = 0; i < D; ++i) {
        mem_free(phis[i]->coef);
        free(phis[i]);
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_coeff357();
        initialized = true;
    }
    if (n > 192) return 0;   /* degree of Φ_357 is 192 */
    return coeff357[(size_t)n];
}
