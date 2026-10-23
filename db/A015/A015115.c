/*
 * A015115  Inverse of 1106th cyclotomic polynomial.
 * https://oeis.org/A015115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = the coefficient of xⁿ in the formal power series
 *            expansion of 1/Φ₁₁₀₆(x), where Φ₁₁₀₆(x) is the 1106‑th
 *            cyclotomic polynomial
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficients repeat with period 1106.  We compute the reciprocal
   series up to one full period by multiplying and dividing polynomials
   with integer coefficients, then store the result in a static array. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------- simple polynomial structure ----------------- */
struct Poly {
    int deg;      /* highest index with non‑zero coefficient */
    seq_t *c;     /* coefficients 0..deg, little‑endian */
};

static struct Poly *poly_create(int deg)
{
    struct Poly *p = (struct Poly *)malloc(sizeof(struct Poly));
    if (!p) return NULL;
    p->deg = deg;
    p->c   = (seq_t *)calloc((size_t)deg + 1, sizeof(seq_t));
    return p;
}

static void poly_free(struct Poly *p)
{
    if (!p) return;
    free(p->c);
    free(p);
}

static struct Poly *poly_mul(const struct Poly *A, const struct Poly *B)
{
    int deg = A->deg + B->deg;
    struct Poly *R = poly_create(deg);
    for (int i = 0; i <= A->deg; ++i) {
        seq_t ai = A->c[i];
        if (ai == 0) continue;
        for (int j = 0; j <= B->deg; ++j) {
            R->c[i + j] += ai * B->c[j];
        }
    }
    return R;
}

/* exact division: A = B * Q ,  assume B is monic and divides A exactly */
static struct Poly *poly_div_exact(const struct Poly *A, const struct Poly *B)
{
    int n = A->deg;
    int m = B->deg;
    int qdeg = n - m;
    struct Poly *Q = poly_create(qdeg);
    seq_t *R = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    for (int i = 0; i <= n; ++i) R[i] = A->c[i];

    for (int k = n; k >= m; --k) {
        seq_t coeff = R[k];
        Q->c[k - m] = coeff;
        if (coeff != 0) {
            for (int j = 0; j <= m; ++j) {
                R[k - m + j] -= coeff * B->c[j];
            }
        }
    }
    free(R);
    return Q;
}

static struct Poly *poly_from_factor(int d)
{
    struct Poly *p = poly_create(d);
    p->c[0] = -1;
    p->c[d] = 1;  /* x^d – 1 */
    return p;
}

/* ----------------- global data for the sequence ----------------- */
#define PERIOD 1106
static seq_t period_arr[PERIOD];
static bool seq_initialized = false;

/* compute Φ1106(x) and its reciprocal series up to one full period */
static void init_sequence(void)
{
    /* divisors of 1106 with μ(1106/d) == +1 : 2, 7, 79, 1106  */
    const int pos_divs[4] = {2, 7, 79, 1106};
    /* divisors of 1106 with μ(1106/d) == -1 : 1, 14, 158, 553  */
    const int neg_divs[4] = {1, 14, 158, 553};

    /* build numerators product (x^d – 1 for +1 divisors) */
    struct Poly *num = poly_create(0);
    num->c[0] = 1;                            /* start with 1 */
    for (int i = 0; i < 4; ++i) {
        struct Poly *f   = poly_from_factor(pos_divs[i]);
        struct Poly *prod = poly_mul(num, f);
        poly_free(num);
        poly_free(f);
        num = prod;
    }

    /* build denominator product (x^d – 1 for –1 divisors) */
    struct Poly *den = poly_create(0);
    den->c[0] = 1;
    for (int i = 0; i < 4; ++i) {
        struct Poly *f   = poly_from_factor(neg_divs[i]);
        struct Poly *prod = poly_mul(den, f);
        poly_free(den);
        poly_free(f);
        den = prod;
    }

    /* Φ1106(x) = num / den  (exact division) */
    struct Poly *phi = poly_div_exact(num, den);
    poly_free(num);
    poly_free(den);

    /* compute reciprocal series: a_0 = 1,  a_k = -∑_{i=1}^{min(k,deg)} φ_i a_{k-i} */
    period_arr[0] = (seq_t)1;
    for (int k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        int max_i = (k <= phi->deg) ? k : phi->deg;
        for (int i = 1; i <= max_i; ++i) {
            sum += phi->c[i] * period_arr[k - i];
        }
        period_arr[k] = -sum;
    }

    poly_free(phi);
    seq_initialized = true;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!seq_initialized) init_sequence();
    return period_arr[n % PERIOD];
}
