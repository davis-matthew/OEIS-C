/*
 * A016405  418th cyclotomic polynomial.
 * https://oeis.org/A016405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 418‑th cyclotomic polynomial
 *            Φ_418(x)
 * Range:     i8 n <= 180, i16 n <= 180, i32 n <= 180, i64 n <= 180,
 *            i128 n <= 180, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Simple polynomial representation */
typedef struct {
    idx_t deg;
    seq_t *coeff;          /* coeff[i] is coefficient of x^i, 0 ≤ i ≤ deg */
} poly_t;

/* Free memory used by a poly_t */
static void free_poly(poly_t *p) {
    if (p->coeff) {
        free(p->coeff);
        p->coeff = NULL;
    }
    p->deg = 0;
}

/* Polynomial multiplication */
static poly_t poly_mul(const poly_t *a, const poly_t *b) {
    poly_t r;
    r.deg = a->deg + b->deg;
    r.coeff = (seq_t*)calloc(r.deg + 1, sizeof(seq_t));
    for (idx_t i = 0; i <= a->deg; ++i) {
        for (idx_t j = 0; j <= b->deg; ++j) {
            r.coeff[i + j] += a->coeff[i] * b->coeff[j];
        }
    }
    return r;
}

/* Exact division of polynomials: num = den * q  (leading coefficient of den is 1) */
static poly_t poly_div_exact(const poly_t *num, const poly_t *den) {
    idx_t m = num->deg;
    idx_t n = den->deg;
    idx_t qdeg = m - n;
    poly_t q;
    q.deg = qdeg;
    q.coeff = (seq_t*)calloc(qdeg + 1, sizeof(seq_t));
    seq_t *rem = (seq_t*)calloc(m + 1, sizeof(seq_t));
    for (idx_t i = 0; i <= m; ++i) rem[i] = num->coeff[i];

    for (idx_t i = m; i >= n; --i) {
        seq_t coeff = rem[i];
        if (!coeff) continue;
        q.coeff[i - n] = coeff;
        for (idx_t j = 0; j <= n; ++j) {
            rem[i - n + j] -= coeff * den->coeff[j];
        }
    }

    while (q.deg > 0 && q.coeff[q.deg] == 0) --q.deg;
    free(rem);
    return q;
}

/* Cache for Φ_d polynomials up to 418 */
static poly_t phi_cache[419];
static bool phi_computed = false;

/* Compute Φ_d for all divisors of 418 and store them */
static void compute_phi_cache(void) {
    if (phi_computed) return;
    for (int i = 0; i < 419; ++i) {
        phi_cache[i].coeff = NULL;
        phi_cache[i].deg = 0;
    }

    /* Φ_1(x) = x - 1 */
    poly_t p1;
    p1.deg = 1;
    p1.coeff = (seq_t*)calloc(2, sizeof(seq_t));
    p1.coeff[0] = -1;
    p1.coeff[1] = 1;
    phi_cache[1] = p1;

    /* Divisors of 418 in ascending order */
    static const idx_t divs[8] = {1, 2, 11, 19, 22, 38, 209, 418};
    const size_t dcnt = 8;

    for (size_t idx = 1; idx < dcnt; ++idx) {
        idx_t d = divs[idx];

        /* Numerator: x^d - 1 */
        poly_t num;
        num.deg = d;
        num.coeff = (seq_t*)calloc(d + 1, sizeof(seq_t));
        num.coeff[0] = -1;
        num.coeff[d] = 1;

        /* Denominator: product of Φ_e for all proper divisors e | d */
        poly_t den;
        den.deg = 0;
        den.coeff = (seq_t*)calloc(1, sizeof(seq_t));
        den.coeff[0] = 1;   /* multiplicative identity */

        for (size_t j = 0; j < idx; ++j) {
            idx_t e = divs[j];
            if (d % e == 0) {
                poly_t tmp = poly_mul(&den, &phi_cache[e]);
                free_poly(&den);
                den = tmp;
            }
        }

        /* Φ_d = num / den */
        poly_t phi_d = poly_div_exact(&num, &den);
        free_poly(&num);
        free_poly(&den);
        phi_cache[d] = phi_d;
    }

    phi_computed = true;
}

/* Main function requested by the problem statement */
seq_t a(idx_t n)
{
    static bool ready = false;
    if (!ready) {
        compute_phi_cache();
        ready = true;
    }
    if (n > phi_cache[418].deg) return 0;
    return phi_cache[418].coeff[n];
}
