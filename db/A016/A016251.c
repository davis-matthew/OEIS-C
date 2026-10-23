/*
 * A016251  Inverse of 2242nd cyclotomic polynomial.
 * https://oeis.org/A016251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in 1 / Φ_2242(x) where Φ_2242 is the
 *            2242‑th cyclotomic polynomial
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Polynomial abstraction ------------------------------------------- */
typedef struct {
    seq_t *c;     /* coefficient array, c[0] constant term */
    size_t len;   /* number of coefficients (degree + 1) */
} poly_t;

/* Allocate an empty polynomial of given length; zero‑initialize. */
static poly_t poly_create(size_t len)
{
    poly_t p;
    p.len = len;
    if (len == 0) {
        p.c = NULL;
        return p;
    }
    p.c = (seq_t*)calloc(len, sizeof(seq_t));
    return p;
}

/* Free memory held by a polynomial. */
static void poly_destroy(poly_t *p)
{
    if (p->c) {
        free(p->c);
        p->c = NULL;
    }
    p->len = 0;
}

/* Multiply a polynomial by (x^k - 1).  Result is a new polynomial. */
static poly_t poly_mul_xk_minus1(const poly_t *old, size_t k)
{
    poly_t res = poly_create(old->len + k);
    for (size_t i = 0; i < old->len; ++i) {
        /* (x^k - 1) * old : contribution to x^i : -old.c[i] */
        res.c[i] -= old->c[i];
        /* contribution to x^(i+k): +old.c[i] */
        res.c[i + k] += old->c[i];
    }
    return res;
}

/* Exact division of polynomial num by monic polynomial den.
   Assumes that the division is exact. */
static poly_t poly_div(const poly_t *num, const poly_t *den)
{
    size_t degA = num->len - 1;
    size_t degB = den->len - 1;
    size_t qlen = degA - degB + 1;
    poly_t Q = poly_create(qlen);
    /* Work on a mutable copy of the numerator */
    seq_t *A = (seq_t*)calloc(num->len, sizeof(seq_t));
    for (size_t i = 0; i < num->len; ++i) A[i] = num->c[i];
    for (int i = (int)degA; i >= (int)degB; --i) {
        seq_t factor = A[i];
        if (factor != 0) {
            Q.c[i - (int)degB] = factor;
            for (size_t j = 0; j <= degB; ++j) {
                A[i - j] -= factor * den->c[degB - j];
            }
        }
    }
    free(A);
    return Q;
}

/* Compute Φ_2242(x) using the product formula for square‑free n.
   Returns a polynomial with degree φ(2242)=1062. */
static poly_t compute_phi_2242(void)
{
    /* Factors of 2242 = 2 * 19 * 59 */
    /* Numerator factors (exponent +1 in Möbius product)              */
    const size_t num_factors[] = {2242, 59, 19, 2};
    const size_t den_factors[] = {1121, 118, 38, 1};

    /* Start with polynomial 1 */
    poly_t num = poly_create(1);
    num.c[0] = 1;
    for (size_t i = 0; i < sizeof(num_factors)/sizeof(num_factors[0]); ++i) {
        poly_t tmp = poly_mul_xk_minus1(&num, num_factors[i]);
        poly_destroy(&num);
        num = tmp;
    }

    poly_t den = poly_create(1);
    den.c[0] = 1;
    for (size_t i = 0; i < sizeof(den_factors)/sizeof(den_factors[0]); ++i) {
        poly_t tmp = poly_mul_xk_minus1(&den, den_factors[i]);
        poly_destroy(&den);
        den = tmp;
    }

    poly_t phi = poly_div(&num, &den);
    poly_destroy(&num);
    poly_destroy(&den);
    return phi;
}

/* Period and cache ----------------------------------------------- */
#define PERIOD 2242
static seq_t seq_cache[PERIOD];

/* Compute the periodic sequence once. */
static void compute_sequence(void)
{
    /* Compute the cyclotomic polynomial Φ_2242 */
    poly_t phi = compute_phi_2242();

    /* Recurrence: a(0)=1, a(n) = -sum_{j=1..min(n,deg)} phi.c[j]*a(n-j) */
    seq_cache[0] = 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t maxj = (n < phi.len - 1) ? n : phi.len - 1;
        for (size_t j = 1; j <= maxj; ++j) {
            sum += phi.c[j] * seq_cache[n - j];
        }
        seq_cache[n] = -sum;
    }

    poly_destroy(&phi);
}

/* External function required by the verifier */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        compute_sequence();
        initialized = true;
    }
    return seq_cache[n % PERIOD];
}
