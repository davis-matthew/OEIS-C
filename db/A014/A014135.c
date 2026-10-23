/*
 * A014135  Inverse of 126th cyclotomic polynomial.
 * https://oeis.org/A014135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of xⁿ in the power series expansion of
 *            1/Φ₁₂₆(x). Recurrence: a(0)=1, for n>0 a(n) = -∑_{k=1}^{φ}
 *            Φ₁₂₆,k * a(n‑k), where Φ₁₂₆,k are the coefficients of the 126‑th
 *            cyclotomic polynomial
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   zero_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/zero_seq_arr.h"

typedef idx_t  idx_t;
typedef seq_t  seq_t;

#define MAXDEG 127   /* safe upper bound for all intermediate polynomials */

static const idx_t DIVS[12] = {
    1, 2, 3, 6, 7, 9, 14, 18, 21, 42, 63, 126
};

typedef struct {
    int  deg;            /* degree (number of non‑zero coefficients – 1) */
    seq_t coeff[MAXDEG]; /* coeff[i] = coefficient of xᶦ   */
} poly_t;

/* Initialise a polynomial to zero */
static void zero_poly(poly_t *p) {
    p->deg = 0;
    for (int i = 0; i < MAXDEG; ++i) p->coeff[i] = 0;
}

/* Multiply two polynomials A (degA) and B (degB); result stored in C. */
static void poly_mul(const seq_t *A, int degA,
                     const seq_t *B, int degB,
                     seq_t *C, int *degC_out)
{
    seq_t tmp[MAXDEG];
    zero_seq_arr(tmp, MAXDEG);
    int  degC = degA + degB;
    for (int i = 0; i <= degA; ++i) {
        if (A[i] == 0) continue;
        for (int j = 0; j <= degB; ++j) {
            if (B[j] == 0) continue;
            tmp[i + j] += A[i] * B[j];
        }
    }
    for (int i = 0; i < MAXDEG; ++i) C[i] = tmp[i];
    *degC_out = degC;
}

/* Compute all cyclotomic polynomials Φ_d for d in DIVS. */
static void compute_phi(poly_t *phi)
{
    for (int idx = 0; idx < 12; ++idx) {
        idx_t d = DIVS[idx];
        if (d == 1) {                     /* Φ₁(x) = x – 1 */
            zero_poly(&phi[1]);
            phi[1].deg = 1;
            phi[1].coeff[0] = -1;
            phi[1].coeff[1] =  1;
            continue;
        }

        /* dividend = xᵈ – 1 */
        seq_t dividend[MAXDEG];
        zero_seq_arr(dividend, MAXDEG);
        dividend[0] = -1;
        dividend[(int)d] = 1;
        int degDividend = (int)d;

        /* product of all proper Φ_e for e | d, e < d */
        seq_t prod[MAXDEG];
        zero_seq_arr(prod, MAXDEG);
        prod[0] = 1;
        int degProd = 0;

        for (int j = 0; j < idx; ++j) {          /* earlier divisors only */
            idx_t e = DIVS[j];
            if (d % e == 0) {
                seq_t newprod[MAXDEG];
                zero_seq_arr(newprod, MAXDEG);
                int newdeg;
                poly_mul(prod, degProd,
                         phi[(int)e].coeff, phi[(int)e].deg,
                         newprod, &newdeg);
                for (int i = 0; i < MAXDEG; ++i) prod[i] = newprod[i];
                degProd = newdeg;
            }
        }

        /* long division: dividend / prod = Φ_d (prod is monic) */
        seq_t rem[MAXDEG];
        for (int i = 0; i < MAXDEG; ++i) rem[i] = dividend[i];
        seq_t quot[MAXDEG];
        zero_seq_arr(quot, MAXDEG);
        int degQuot = degDividend - degProd;

        for (int i = degDividend; i >= degProd; --i) {
            seq_t c = rem[i];
            quot[i - degProd] = c;
            if (c != 0) {
                for (int k = 0; k <= degProd; ++k) {
                    rem[i - degProd + k] -= c * prod[k];
                }
            }
        }

        zero_poly(&phi[(int)d]);
        phi[(int)d].deg = degQuot;
        for (int i = 0; i <= degQuot; ++i) {
            phi[(int)d].coeff[i] = quot[i];
        }
    }
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    static bool          initialized = false;
    static seq_t         cache[126];          /* period length 126 */
    static poly_t        phi[127];            /* polynomials Φ_d */

    if (!initialized) {
        for (int i = 0; i < 127; ++i) zero_poly(&phi[i]);   /* precaution */
        compute_phi(phi);

        const poly_t *phi126 = &phi[126];
        int deg = phi126->deg;

        cache[0] = 1;
        for (int i = 1; i < 126; ++i) {
            seq_t sum = 0;
            for (int k = 1; k <= deg; ++k) {
                if (i - k >= 0) {
                    sum += phi126->coeff[k] * cache[i - k];
                }
            }
            cache[i] = -sum;
        }
        initialized = true;
    }
    return cache[(size_t)(n % 126u)];
}
