/*
 * A016199  Inverse of 2190th cyclotomic polynomial.
 * https://oeis.org/A016199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A016199 (inverse of the 2190th cyclotomic polynomial)
 *
 *  a(n) is the coefficient of x^n in the formal power series expansion of
 *      1 / Φ_2190(x)
 *
 *  The coefficients are periodic with period 2190.  They are computed
 *  by first building the 2190th cyclotomic polynomial Φ_2190(x) via
 *  the standard recurrence
 *
 *      Φ_n(x) = (x^n - 1) / Π_{d|n, d<n} Φ_d(x)
 *
 *  Once Φ_2190 has been obtained, the coefficients a(k) for 0 ≤ k < 2190
 *  are obtained from the linear recurrence
 *
 *      Φ_0·a(k) + Φ_1·a(k-1) + ⋯ + Φ_d·a(k-d) = 0     (k ≥ 1)
 *
 *  where Φ_0 = 1 and d = deg Φ_2190 = 576.  The period array is then
 *  reused for any n (a(n) = period[n mod 2190]).
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Polynomial utilities ---------- */

typedef struct { size_t len; seq_t *coef; } poly_t;

static poly_t *poly_new(size_t len)
{
    poly_t *p = (poly_t*)malloc(sizeof(poly_t));
    p->len = len;
    p->coef = (seq_t*)calloc(len, sizeof(seq_t));
    return p;
}

static void poly_free(poly_t *p)
{
    if (!p) return;
    free(p->coef);
    free(p);
}

static poly_t *poly_mul(const poly_t *A, const poly_t *B)
{
    size_t na = A->len, nb = B->len;
    poly_t *C = poly_new(na + nb - 1);
    for (size_t i = 0; i < na; ++i)
        if (A->coef[i])
            for (size_t j = 0; j < nb; ++j)
                if (B->coef[j])
                    C->coef[i + j] += A->coef[i] * B->coef[j];
    return C;
}

/* exact division of num by den; both have integer coefficients, den is monic. */
static poly_t *poly_div_exact(const poly_t *num, const poly_t *den)
{
    size_t deg_num = num->len - 1;
    size_t deg_den = den->len - 1;
    size_t qlen = deg_num - deg_den + 1;
    poly_t *Q = poly_new(qlen);
    seq_t *r = (seq_t*)malloc((deg_num + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= deg_num; ++i) r[i] = num->coef[i];

    for (long k = (long)deg_num; k >= (long)deg_den; --k) {
        seq_t q = r[k] / den->coef[deg_den];
        if (q)
            Q->coef[k - (long)deg_den] = q;
        for (size_t j = 0; j <= deg_den; ++j)
            r[k - (long)deg_den + j] -= q * den->coef[j];
    }
    free(r);
    return Q;
}

/* ---------- Build the period array ---------- */

static void build_period(seq_t *period)
{
    /* 1. list all divisors of 2190 in ascending order */
    const idx_t divs[] = {1,2,3,5,6,10,15,30,73,146,219,365,438,730,1095,2190};
    const size_t div_count = sizeof(divs) / sizeof(divs[0]);   /* 16 */

    /* 2. compute Φ_d for each divisor d */
    poly_t *phi[div_count];
    for (size_t idx = 0; idx < div_count; ++idx) {
        idx_t d = divs[idx];
        if (d == 1) {
            /* Φ_1(x) = x - 1 */
            poly_t *p = poly_new(2);
            p->coef[0] = -1;
            p->coef[1] = 1;
            phi[0] = p;
        } else {
            /* Φ_d(x) = (x^d - 1) / Π_{d'|d, d'<d} Φ_{d'}(x) */
            poly_t *num = poly_new(d + 1);
            num->coef[0] = -1;
            num->coef[d] = 1;

            /* build denominator as product of all Φ_{d'} that divide d */
            poly_t *den = poly_new(1);
            den->coef[0] = 1;
            for (size_t j = 0; j < idx; ++j) {
                if (d % divs[j] == 0) {
                    poly_t *tmp = poly_mul(den, phi[j]);
                    poly_free(den);
                    den = tmp;
                }
            }

            poly_t *pd = poly_div_exact(num, den);
            poly_free(num);
            poly_free(den);
            phi[idx] = pd;
        }
    }

    /* 3. product of all Φ_d for d < 2190 (exclude the last divisor) */
    poly_t *prod = poly_new(1);
    prod->coef[0] = 1;
    for (size_t j = 0; j < div_count - 1; ++j) {
        poly_t *tmp = poly_mul(prod, phi[j]);
        poly_free(prod);
        prod = tmp;
    }

    /* 4. Φ_2190(x) = (x^2190 - 1) / prod */
    const idx_t N = 2190;
    poly_t *num = poly_new(N + 1);
    num->coef[0] = -1;
    num->coef[N] = 1;

    poly_t *phi_n = poly_div_exact(num, prod);
    poly_free(num);
    poly_free(prod);

    /* 5. copy its coefficients for recurrence */
    size_t deg = phi_n->len - 1;          /* should be 576 */
    seq_t *phi_coeff = (seq_t*)malloc((deg + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= deg; ++i) phi_coeff[i] = phi_n->coef[i];

    /* 6. compute the period using the linear recurrence */
    for (size_t k = 0; k < 2190; ++k) {
        if (k == 0) {
            period[0] = 1;
        } else {
            seq_t sum = 0;
            size_t lim = (k < deg ? k : deg);
            for (size_t i = 1; i <= lim; ++i) {
                sum += phi_coeff[i] * period[k - i];
            }
            period[k] = -sum;
        }
    }

    /* 7. free temporary memory */
    for (size_t j = 0; j < div_count; ++j) poly_free(phi[j]);
    poly_free(phi_n);
    free(phi_coeff);
}

/* ---------- Public function ---------- */

seq_t a(idx_t n)
{
    static seq_t period[2190];
    static bool initialized = false;
    if (!initialized) {
        build_period(period);
        initialized = true;
    }
    return period[(size_t)(n % 2190)];
}
