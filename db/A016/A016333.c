/*
 * A016333  138th cyclotomic polynomial.
 * https://oeis.org/A016333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 44, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation computes the coefficients of the 138th
    cyclotomic polynomial Φ138(x) by the standard definition:
    Φn(x) = (x^n-1)/∏_{d|n,d<n} Φd(x).
    The function a(n) returns the coefficient of x^n in Φ138(x).
    For n > 44 the coefficient is 0.  The polynomial has degree
    φ(138)=44.  The implementation works for any build that
    supplies seq_t and idx_t.  */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ---------- helper structures ---------- */
typedef struct {
    int deg;                     /* degree (highest exponent with non‑zero coefficient) */
    int64_t coeff[139];          /* coefficients 0 .. 138; degree never exceeds 138 */
} poly_t;

/* ---------- polynomial multiplication: C = A * B ---------- */
static void poly_mul(const poly_t *A, const poly_t *B, poly_t *C)
{
    memset(C->coeff, 0, sizeof(C->coeff));
    C->deg = 0;
    for (int i = 0; i <= A->deg; ++i) {
        if (A->coeff[i] == 0) continue;
        for (int j = 0; j <= B->deg; ++j) {
            if (B->coeff[j] == 0) continue;
            int64_t prod = A->coeff[i] * B->coeff[j];
            int k = i + j;
            C->coeff[k] += prod;
            if (C->coeff[k] && k > C->deg) C->deg = k;
        }
    }
}

/* ---------- exact polynomial division: Q = V / D ----------
    D must be monic (leading coefficient 1).  V must be divisible by D exactly. */
static void poly_divexact(const poly_t *V, const poly_t *D, poly_t *Q)
{
    int vdeg = V->deg;
    int ddeg = D->deg;
    if (vdeg < ddeg) {        /* should not happen for correct inputs */
        Q->deg = 0;
        memset(Q->coeff, 0, sizeof(Q->coeff));
        return;
    }

    int qdeg = vdeg - ddeg;
    memset(Q->coeff, 0, sizeof(Q->coeff));
    Q->deg = qdeg;

    /* tmp will hold the current dividend during the division */
    int64_t tmp[139];
    memcpy(tmp, V->coeff, sizeof(tmp));

    for (int i = vdeg; i >= ddeg; --i) {
        int64_t coeff = tmp[i];            /* because D is monic */
        int idx = i - ddeg;                /* position of this term in the quotient */
        Q->coeff[idx] = coeff;
        for (int j = 0; j <= ddeg; ++j) {
            tmp[idx + j] -= coeff * D->coeff[j];
        }
    }
}

/* ---------- compute Φ138 and store coefficients ---------- */
static void compute_phi138(poly_t *phi138)
{
    /* 138 = 2 * 3 * 23, divisors sorted increasingly. */
    const unsigned int divs[] = {1, 2, 3, 6, 23, 46, 69, 138};
    const size_t Nd = sizeof(divs) / sizeof(divs[0]);

    poly_t polys[Nd];
    memset(polys, 0, sizeof(polys));

    for (size_t i = 0; i < Nd; ++i) {
        unsigned int d = divs[i];

        if (d == 1) {                       /* Φ1(x) = x - 1 */
            polys[i].deg = 1;
            polys[i].coeff[0] = -1;
            polys[i].coeff[1] = 1;
            continue;
        }

        /* Numerator: V(x) = x^d - 1 */
        poly_t V;
        memset(V.coeff, 0, sizeof(V.coeff));
        V.deg = d;
        V.coeff[0] = -1;
        V.coeff[d] = 1;

        /* Product of all Φ_e for e | d, e < d */
        poly_t prod;
        memset(prod.coeff, 0, sizeof(prod.coeff));
        prod.deg = 0;
        bool first = true;

        for (size_t j = 0; j < i; ++j) {
            if (d % divs[j] == 0) {
                if (first) {
                    prod = polys[j];
                    first = false;
                } else {
                    poly_t tmp;
                    poly_mul(&prod, &polys[j], &tmp);
                    prod = tmp;
                }
            }
        }

        /* Compute Φ_d(x) = (x^d - 1) / prod */
        poly_t Q;
        poly_divexact(&V, &prod, &Q);
        polys[i] = Q;
    }

    /* Copy Φ138 into the provided output structure */
    *phi138 = polys[Nd - 1];
}

/* ---------- global cache for the coefficients ---------- */
static seq_t coefficients[139];
static size_t degree_of_phi138 = 0;
static bool initialized = false;

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Compute Φ138 once */
        poly_t phi;
        compute_phi138(&phi);
        degree_of_phi138 = (size_t)phi.deg;
        for (size_t i = 0; i <= degree_of_phi138; ++i) {
            /* Final coefficients are -1, 0, or 1; cast safely. */
            coefficients[i] = (seq_t)phi.coeff[i];
        }
        /* Pad the rest with zeroes (they are already zero). */
        initialized = true;
    }

    if (n <= degree_of_phi138) {
        return coefficients[(size_t)n];
    }
    return (seq_t)0;
}
