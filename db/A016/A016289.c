/*
 * A016289  Inverse of 2280th cyclotomic polynomial.
 * https://oeis.org/A016289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   Let Φ(x) = Φ₍2280₎(x) = Σ coeff[k]·xᵏ be the 2280‑th cyclotomic
 *            polynomial. The sequence a(n) is defined by a(0)=1 and for n>0
 *            Σ_{k=0}^{deg(Φ)} coeff[k]·a(n‑k) = 0, i.e. a(n) =
 *            –Σ_{k=1}^{deg(Φ)} coeff[k]·a(n‑k). The sequence is periodic with
 *            period 2280
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Period of the sequence. */
#define PERIOD 2280
/* φ(2280) = 576, the degree of Φ₍2280₎. */
#define DEG_MAX 576
/* Maximum number of divisors of 2280. */
#define MAX_DIV 64
/* Maximum polynomial degree encountered (≤2280). */
#define MAX_DEG 2280

/* Polynomial with integer coefficients.  Coefficients are stored in
   int64_t to avoid overflow during construction. */
typedef struct {
    int   deg;                     /* highest non‑zero coefficient index */
    long long coeff[MAX_DEG + 1];   /* coeff[i] is coefficient of xⁱ */
} Poly;

/* Multiply two polynomials A and B, store result in R. */
static void poly_mul(const Poly *A, const Poly *B, Poly *R)
{
    int da = A->deg, db = B->deg;
    int dr = da + db;
    if (dr > MAX_DEG) dr = MAX_DEG;
    for (int i = 0; i <= dr; ++i) R->coeff[i] = 0;
    for (int i = 0; i <= da; ++i) {
        if (A->coeff[i] == 0) continue;
        for (int j = 0; j <= db; ++j) {
            R->coeff[i + j] += A->coeff[i] * B->coeff[j];
        }
    }
    R->deg = dr;
}

/* Divide monic polynomial NUM by MONIC MON and store quotient in Q.  The
   remainder is expected to be zero.  All polynomials are assumed to have
   non‑negative degrees and coefficients within int64_t. */
static void poly_div(const Poly *NUM, const Poly *MON, Poly *Q)
{
    int nd = NUM->deg;
    int md = MON->deg;
    int qd = nd - md;
    long long tmp[MAX_DEG + 1];
    for (int i = 0; i <= nd; ++i) tmp[i] = NUM->coeff[i];
    for (int i = 0; i <= qd; ++i) Q->coeff[i] = 0;
    for (int i = nd; i >= md; --i) {
        long long c = tmp[i];
        if (c == 0) continue;
        int shift = i - md;
        Q->coeff[shift] = c;
        for (int j = 0; j <= md; ++j) {
            tmp[shift + j] -= c * MON->coeff[j];
        }
    }
    Q->deg = qd;
    /* Zero out remaining higher entries. */
    for (int i = qd + 1; i <= MAX_DEG; ++i) Q->coeff[i] = 0;
}

/* Build all cyclotomic polynomials Φ_d for d | 2280 in ascending order. */
static void build_cyclotomics(Poly polys[MAX_DIV], int divs[MAX_DIV], int *divcnt)
{
    *divcnt = 0;

    /* Enumerate divisors of 2280. */
    for (int i = 1; i <= PERIOD; ++i)
        if (PERIOD % i == 0)
            divs[(*divcnt)++] = i;

    /* Compute Φ_d for each divisor in ascending order. */
    for (int idx = 0; idx < *divcnt; ++idx) {
        int d = divs[idx];
        Poly *p = &polys[idx];
        if (d == 1) { /* Φ₁(x) = x – 1 */
            for (int i = 0; i <= MAX_DEG; ++i) p->coeff[i] = 0;
            p->coeff[0] = -1;
            p->coeff[1] = 1;
            p->deg = 1;
            continue;
        }

        /* Numerator: xᵈ – 1. */
        Poly num;
        for (int i = 0; i <= MAX_DEG; ++i) num.coeff[i] = 0;
        num.coeff[0] = -1;
        num.coeff[d] = 1;
        num.deg = d;

        /* Denominator: product of Φ_e for e|d, e<d. */
        Poly den;
        for (int i = 0; i <= MAX_DEG; ++i) den.coeff[i] = 0;
        den.coeff[0] = 1;
        den.deg = 0;
        for (int j = 0; j < idx; ++j) {
            if (d % divs[j] == 0) {
                Poly tmp;
                poly_mul(&den, &polys[j], &tmp);
                den = tmp;
            }
        }

        /* Division: Φ_d = (xᵈ – 1) / den. */
        Poly quo;
        poly_div(&num, &den, &quo);
        *p = quo;
    }
}

/* Compute one full period of the sequence and store it in period_vals. */
static void precompute_sequence(seq_t period_vals[PERIOD])
{
    Poly polys[MAX_DIV];
    int divs[MAX_DIV];
    int divcnt;

    build_cyclotomics(polys, divs, &divcnt);

    /* Φ₍2280₎ is the polynomial for divisor 2280, which is the last one. */
    Poly *phi = &polys[divcnt - 1];

    /* Establish initial value a(0) = 1. */
    period_vals[0] = 1;

    /* Use the recurrence to compute the remaining values in the period. */
    for (int i = 1; i < PERIOD; ++i) {
        long long sum = 0;
        int maxk = phi->deg;
        if (maxk > i) maxk = i;
        for (int k = 1; k <= maxk; ++k) {
            sum += phi->coeff[k] * (long long)period_vals[i - k];
        }
        period_vals[i] = (seq_t)-sum;
    }
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period_vals[PERIOD];

    if (!initialized) {
        precompute_sequence(period_vals);
        initialized = true;
    }
    return period_vals[n % PERIOD];
}
