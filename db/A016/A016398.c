/*
 * A016398  390th cyclotomic polynomial.
 * https://oeis.org/A016398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = coefficient of x^n in Φ₃₉₀(x)  (390th cyclotomic polynomial) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* list of all positive divisors of 390, sorted increasingly */
#define NUM_DIV 16
static const idx_t divs[NUM_DIV] = {1, 2, 3, 5, 6, 10, 13, 15, 26, 30,
                                    39, 65, 78, 130, 195, 390};

/* maximum degree needed for intermediate products: deg(x^390-1) = 390 */
#define MAX_DEG 390

typedef struct {
    size_t deg;
    seq_t coeff[MAX_DEG + 1];
} poly;

static poly polys[NUM_DIV];
static bool built = false;

/* set a polynomial to zero */
static void poly_zero(poly *p) {
    p->deg = 0;
    for (size_t i = 0; i <= MAX_DEG; ++i) p->coeff[i] = 0;
}

/* set a polynomial to one */
static void poly_one(poly *p) {
    poly_zero(p);
    p->coeff[0] = 1;
}

/* multiplication of polynomials a and b, store in res */
static void poly_mul(const poly *a, const poly *b, poly *res) {
    poly_zero(res);
    res->deg = a->deg + b->deg;
    for (size_t i = 0; i <= a->deg; ++i) {
        if (a->coeff[i] == 0) continue;
        for (size_t j = 0; j <= b->deg; ++j) {
            if (b->coeff[j] == 0) continue;
            res->coeff[i + j] += a->coeff[i] * b->coeff[j];
        }
    }
    while (res->deg > 0 && res->coeff[res->deg] == 0) --res->deg;
}

/* exact division: res = num / den   (den is monic) */
static void poly_div_exact(const poly *num, const poly *den, poly *res) {
    seq_t rem[MAX_DEG + 1];
    for (size_t i = 0; i <= MAX_DEG; ++i) rem[i] = 0;
    for (size_t i = 0; i <= num->deg; ++i) rem[i] = num->coeff[i];

    size_t deg_num = num->deg;
    size_t deg_den = den->deg;

    if (deg_num < deg_den) {
        poly_zero(res);
        return;
    }

    seq_t q[MAX_DEG + 1] = {0};
    for (int i = (int)deg_num; i >= (int)deg_den; --i) {
        seq_t lead = rem[i];
        if (lead != 0) {
            size_t pos = i - deg_den;
            q[pos] = lead;
            for (size_t j = 0; j <= deg_den; ++j) {
                rem[pos + j] -= lead * den->coeff[j];
            }
        }
    }

    size_t q_deg = deg_num - deg_den;
    while (q_deg > 0 && q[q_deg] == 0) --q_deg;

    res->deg = q_deg;
    for (size_t i = 0; i <= MAX_DEG; ++i) res->coeff[i] = q[i];
}

/* find the index of a divisor in the divs array */
static int idx_of_div(idx_t d) {
    for (int i = 0; i < NUM_DIV; ++i) {
        if (divs[i] == d) return i;
    }
    return -1;
}

/* build all Φ_d for d | 390 */
static void build_phi_390(void) {
    for (int idx = 0; idx < NUM_DIV; ++idx) {
        idx_t d = divs[idx];

        /* xn_minus_1 = x^d - 1 */
        poly xn;
        poly_zero(&xn);
        xn.deg = (size_t)d;
        xn.coeff[0] = -1;
        xn.coeff[(size_t)d] = 1;

        /* product of Φ_f for all proper divisors f of d */
        poly prod;
        poly_one(&prod);
        for (int j = 0; j < idx; ++j) {
            if (d % divs[j] == 0) {
                poly tmp;
                poly_mul(&prod, &polys[j], &tmp);
                prod = tmp;  /* struct copy */
            }
        }

        /* Φ_d = (x^d - 1) / prod */
        poly quo;
        poly_div_exact(&xn, &prod, &quo);
        polys[idx] = quo;
    }
    built = true;
}

/*
 * Sequence entry a(n): coefficient of x^n in Φ₃₉₀(x).
 */
seq_t a(idx_t n) {
    if (!built) build_phi_390();
    int id = idx_of_div(390);
    if (id < 0) return 0;  /* should not happen */

    poly *phi = &polys[id];
    if (n <= (idx_t)phi->deg)          /* idx_t is unsigned, safe cast */
        return phi->coeff[(size_t)n];
    return 0;
}
