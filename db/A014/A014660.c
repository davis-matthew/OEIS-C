/*
 * A014660  Inverse of 651st cyclotomic polynomial.
 * https://oeis.org/A014660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = –[xⁿ]∏_{d∣651,d<651}Φ_d(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The product of all proper cyclotomic polynomials has degree 279 and
    coefficients in {–1,0,1}.  The sequence is periodic with period 651,
    equal to the negative of these coefficients (zeros for indices > 279). */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct { size_t deg; seq_t *c; } Poly;

/* create a polynomial with all coefficients initialized to 0 */
static Poly *poly_new(size_t deg) {
    Poly *p = (Poly*)malloc(sizeof(Poly));
    p->deg = deg;
    p->c = (seq_t*)calloc(deg + 1, sizeof(seq_t));
    return p;
}

/* free a polynomial */
static void poly_free(Poly *p) {
    if (!p) return;
    free(p->c);
    free(p);
}

/* multiply two polynomials */
static Poly *poly_mul(const Poly *a, const Poly *b) {
    size_t da = a->deg;
    size_t db = b->deg;
    size_t deg = da + db;
    Poly *r = poly_new(deg);
    for (size_t i = 0; i <= da; ++i) {
        if (a->c[i] == 0) continue;
        for (size_t j = 0; j <= db; ++j) {
            if (b->c[j] == 0) continue;
            r->c[i + j] += a->c[i] * b->c[j];
        }
    }
    /* trim leading zeros */
    while (deg > 0 && r->c[deg] == 0) --deg;
    r->deg = deg;
    return r;
}

/* exact division A / B, where B divides A (leading coeff of B is 1) */
static Poly *poly_div(const Poly *A, const Poly *B) {
    size_t n = A->deg;
    size_t m = B->deg;
    if (n < m) {   /* result zero */
        Poly *q = poly_new(0);
        q->c[0] = 0;
        return q;
    }
    size_t qdeg = n - m;
    Poly *Q = poly_new(qdeg);
    seq_t *rem = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    memcpy(rem, A->c, (n + 1) * sizeof(seq_t));
    for (int i = (int)n; i >= (int)m; --i) {
        seq_t coeff = rem[i];
        if (coeff != 0) {
            Q->c[i - (int)m] = coeff;
            for (size_t j = 0; j <= m; ++j) {
                rem[(i - (int)m) + j] -= coeff * B->c[j];
            }
        }
    }
    free(rem);
    while (qdeg > 0 && Q->c[qdeg] == 0) --qdeg;
    Q->deg = qdeg;
    return Q;
}

/* Build product of proper divisor cyclotomics: Q(x). */
static void compute_period(seq_t *period) {
    const idx_t divisors[] = {1,3,7,21,31,93,217,651};
    const size_t DCOUNT = sizeof(divisors)/sizeof(divisors[0]);
    const idx_t PERIOD = 651;

    /* Store Φ_d for proper divisors (except 651). */
    Poly *phis[7];
    for (size_t i = 0; i < 7; ++i) phis[i] = NULL;

    /* Cumulative product of all Φ_d processed so far. */
    Poly *cum = poly_new(0);
    cum->c[0] = (seq_t)1;   /* start with 1 */

    for (size_t i = 0; i < 7; ++i) {
        idx_t d = divisors[i];

        /* P(x) = x^d – 1. */
        Poly *P = poly_new(d);
        P->c[0] = (seq_t)-1;
        P->c[d] = (seq_t)1;

        /* prod = product of Φ_e for e|d, e<d. */
        Poly *prod = poly_new(0);
        prod->c[0] = (seq_t)1;
        for (size_t j = 0; j < i; ++j) {
            if (d % divisors[j] == 0) {
                Poly *tmp = poly_mul(prod, phis[j]);
                poly_free(prod);
                prod = tmp;
            }
        }

        /* Φ_d = P / prod. */
        Poly *Phi = poly_div(P, prod);
        poly_free(P);
        poly_free(prod);
        phis[i] = Phi;

        /* cum *= Φ_d. */
        Poly *tmp = poly_mul(cum, Phi);
        poly_free(cum);
        cum = tmp;
    }

    /* cum == Q(x).  Copy coefficients as the negative of the sequence. */
    for (size_t i = 0; i <= cum->deg && i < PERIOD; ++i) {
        period[i] = -(cum->c[i]);   /* negate the coefficient */
    }
    for (size_t i = cum->deg + 1; i < PERIOD; ++i) period[i] = (seq_t)0;

    /* free memory */
    for (size_t i = 0; i < 7; ++i) poly_free(phis[i]);
    poly_free(cum);
}

/* Main sequence function */
seq_t a(idx_t n) {
    static seq_t period[651];
    static bool initialized = false;
    if (!initialized) {
        compute_period(period);
        initialized = true;
    }
    idx_t idx = n % 651;
    return period[idx];
}
