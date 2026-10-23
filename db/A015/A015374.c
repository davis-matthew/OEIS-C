/*
 * A015374  Inverse of 1365th cyclotomic polynomial.
 * https://oeis.org/A015374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the reciprocal series of the
 *            1365-th cyclotomic polynomial. The sequence is periodic with
 *            period length 1365
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    idx_t deg;
    seq_t *c;     /* coefficients, c[0] is the constant term */
} Poly;

/* Create a polynomial of given degree, all coefficients initialized to 0. */
static Poly *poly_create(idx_t deg)
{
    Poly *p = (Poly *)malloc(sizeof(Poly));
    p->deg = deg;
    p->c = (seq_t *)calloc(deg + 1, sizeof(seq_t));
    return p;
}

/* Free a polynomial. */
static void poly_free(Poly *p)
{
    if (p) {
        free(p->c);
        free(p);
    }
}

/* Multiply two polynomials and return a new polynomial. */
static Poly *poly_mul(const Poly *a, const Poly *b)
{
    idx_t deg = a->deg + b->deg;
    Poly *r = poly_create(deg);
    for (int i = 0; i <= (int)a->deg; ++i) {
        for (int j = 0; j <= (int)b->deg; ++j) {
            r->c[i + j] += a->c[i] * b->c[j];
        }
    }
    return r;
}

/* Divide numerator by monic divisor.  Returns quotient.  Assumes
   divisor divides numerator exactly. */
static Poly *poly_div_monically(const Poly *num, const Poly *den)
{
    int deg_n = (int)num->deg;
    int deg_d = (int)den->deg;
    int deg_q = deg_n - deg_d;
    Poly *q = poly_create(deg_q);

    /* Work on a copy of the numerator. */
    Poly *rem = poly_create(num->deg);
    for (idx_t i = 0; i <= (idx_t)num->deg; ++i)
        rem->c[i] = num->c[i];

    for (int i = deg_n; i >= deg_d; --i) {
        seq_t coeff = rem->c[i];
        q->c[i - deg_d] = coeff;
        if (coeff != 0) {
            for (int j = 0; j <= deg_d; ++j) {
                rem->c[i - deg_d + j] -= coeff * den->c[j];
            }
        }
    }

    poly_free(rem);
    return q;
}

/* Compute the sequence values at indices 0..1364 and cache them. */
static void init_cache(seq_t *cache)
{
    /* Divisors of 1365 = 3·5·7·13 in ascending order. */
    const idx_t divisors[16] = {
        1, 3, 5, 7, 13, 15, 21, 35, 39, 65,
        91, 105, 195, 273, 455, 1365
    };
    const int num_div = 16;
    Poly *phi[16];

    /* Compute Φ_d for each divisor d. */
    for (int i = 0; i < num_div; ++i) {
        idx_t d = divisors[i];
        if (d == 1) {            /* Φ_1(x) = x – 1 */
            phi[i] = poly_create(1);
            phi[i]->c[0] = -1;
            phi[i]->c[1] = 1;
            continue;
        }

        /* Product of Φ_e for all proper divisors e of d. */
        Poly *prod = poly_create(0);
        prod->c[0] = 1;          /* polynomial "1" */

        for (int j = 0; j < i; ++j) {
            if (d % divisors[j] == 0) {
                Poly *tmp = poly_mul(prod, phi[j]);
                poly_free(prod);
                prod = tmp;
            }
        }

        /* Numerator: x^d – 1. */
        Poly *num = poly_create(d);
        num->c[0] = -1;
        num->c[d] = 1;

        /* Quotient gives Φ_d(x). */
        phi[i] = poly_div_monically(num, prod);

        poly_free(num);
        poly_free(prod);
    }

    /* φ = Φ_1365(x) (last element). */
    Poly *phi_n = phi[15];
    int deg = (int)phi_n->deg;

    /* Compute the coefficients of the reciprocal series. */
    cache[0] = 1;
    for (idx_t k = 1; k < 1365; ++k) {
        seq_t s = 0;
        int upper = (k <= (idx_t)deg) ? (int)k : deg;
        for (int j = 1; j <= upper; ++j) {
            s += phi_n->c[j] * cache[k - j];
        }
        cache[k] = -s;
    }

    /* Free all temporary polynomials. */
    for (int i = 0; i < num_div; ++i)
        poly_free(phi[i]);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t cache[1365];

    if (!ready) {
        init_cache(cache);
        ready = true;
    }
    return cache[n % 1365];
}
