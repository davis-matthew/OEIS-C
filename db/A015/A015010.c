/*
 * A015010  Inverse of 1001st cyclotomic polynomial.
 * https://oeis.org/A015010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -r[n mod 1001] where r is the coefficient array of the
 *            polynomial (x^1001 - 1) / Φ_1001(x). The sequence is periodic of
 *            period 1001 and takes values -1, 0, 1
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Maximum degree needed for intermediate polynomials. */
#define MAX_D 1001          /* period */
#define MAX_P 1001          /* maximum degree that any polynomial will reach */

/* Helper structure for polynomials.  The degree is at most MAX_P. */
typedef struct {
    idx_t deg;             /* actual degree of the polynomial */
    seq_t coeff[MAX_P + 1];/* coefficients in increasing degree order */
} Poly;

/* Zero a polynomial up to a given degree. */
static void poly_init(Poly *p, idx_t d)
{
    p->deg = d;
    for (idx_t i = 0; i <= d; ++i)
        p->coeff[i] = 0;
}

/* Copy polynomial src to dst.  dst must be large enough. */
static void poly_copy(const Poly *src, Poly *dst)
{
    dst->deg = src->deg;
    for (idx_t i = 0; i <= src->deg; ++i)
        dst->coeff[i] = src->coeff[i];
}

/* Multiply A and B into result.  Result must be large enough. */
static void poly_mul(const Poly *A, const Poly *B, Poly *result)
{
    idx_t ma = A->deg, mb = B->deg;
    idx_t mr = ma + mb;
    poly_init(result, mr);
    for (idx_t i = 0; i <= ma; ++i) {
        for (idx_t j = 0; j <= mb; ++j) {
            result->coeff[i + j] += A->coeff[i] * B->coeff[j];
        }
    }
}

/* Divide D by V (V is monic) into quotient Q.  It is assumed that the
   division is exact and that V is monic (leading coefficient 1). */
static void poly_div_monic(const Poly *D, const Poly *V, Poly *Q)
{
    idx_t m = D->deg;
    idx_t v = V->deg;
    idx_t qdeg = m - v;
    /* Work array R = D */
    static seq_t R[MAX_P + 1];
    for (idx_t i = 0; i <= m; ++i)
        R[i] = D->coeff[i];
    /* Initialize Q */
    for (idx_t i = 0; i <= qdeg; ++i)
        Q->coeff[i] = 0;
    Q->deg = qdeg;
    /* Long division */
    for (int i = (int)m; i >= (int)v; --i) {
        seq_t c = R[i];
        Q->coeff[i - v] = c;
        if (c != 0) {
            for (int j = 0; j <= (int)v; ++j) {
                R[i - v + j] -= c * V->coeff[j];
            }
        }
    }
}

/* Precomputation of the periodic coefficients for period 1001. */
static void init_period_array(seq_t *period_r)
{
    /* Divisors of 1001 in increasing order. */
    static const idx_t divs[] = {1, 7, 11, 13, 77, 91, 143, 1001};
    const idx_t num_divs = sizeof(divs) / sizeof(divs[0]);
    Poly polys[num_divs];
    /* Compute Φ_d for every divisor d. */
    for (idx_t j = 0; j < num_divs; ++j) {
        idx_t d = divs[j];
        if (d == 1) {
            /* Φ_1(x) = x - 1 */
            poly_init(&polys[j], 1);
            polys[j].coeff[0] = -1;
            polys[j].coeff[1] = 1;
        } else {
            /* Product of all Φ_e for e|d, e<d. */
            Poly prod;
            poly_init(&prod, 0);
            prod.coeff[0] = 1;       /* constant polynomial 1 */
            for (idx_t k = 0; k < j; ++k) {
                if (d % divs[k] == 0) {
                    Poly tmp;
                    poly_mul(&prod, &polys[k], &tmp);
                    poly_copy(&tmp, &prod);
                }
            }
            /* P(x) = x^d - 1 */
            Poly P;
            poly_init(&P, d);
            P.coeff[0] = -1;
            P.coeff[d] = 1;
            /* Φ_d = P / prod (exact division, prod is monic) */
            poly_div_monic(&P, &prod, &polys[j]);
        }
    }

    /* Φ_1001 is the last polynomial. */
    const Poly *phi = &polys[num_divs - 1];
    idx_t phi_deg = phi->deg;            /* should be 720 */
    idx_t R_deg = 1001 - phi_deg;        /* 281 */

    /* Compute coefficients r[0..R_deg] of the numerator R(x). */
    for (idx_t i = 0; i <= 1000; ++i)
        period_r[i] = 0;                 /* initialise rest to 0 */

    for (idx_t k = 0; k <= R_deg; ++k) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= phi_deg && i <= k; ++i) {
            sum += phi->coeff[i] * period_r[k - i];
        }
        seq_t c = 0;
        if (k == 0)
            c = -1;
        period_r[k] = c - sum;          /* φ[0] = 1, so no division needed */
    }
}

/* Sequence function for A015010. */
seq_t a(idx_t n)
{
    /* Static array to hold precomputed coefficients. */
    static seq_t period_r[1001];
    static int ready = 0;
    if (!ready) {
        init_period_array(period_r);
        ready = 1;
    }
    idx_t idx = n % 1001ULL;
    return -period_r[idx];
}
