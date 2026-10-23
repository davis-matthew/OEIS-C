/*
 * A014111  Inverse of 102nd cyclotomic polynomial.
 * https://oeis.org/A014111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n>0: * a(n) = -
 *            (Φ102[1]*a(n-1)+Φ102[2]*a(n-2)+…+Φ102[32]*a(n-32)), * where
 *            Φ102(x) is the 102nd cyclotomic polynomial * and Φ102[i] is its
 *            coefficient of x^i (Φ102[0]=1). The sequence * repeats with
 *            period 102
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool, true, false */

/* Auxiliary polynomial type.  Max degree needed is 102. */
typedef struct {
    size_t deg;                 /* actual degree (highest non‑zero coeff) */
    seq_t coeff[103];           /* coeff[0] + coeff[1]x + … + coeff[deg]x^deg */
} poly_t;

/* poly_mul(A,B,C)  :  C = A * B */
static void poly_mul(const poly_t *A, const poly_t *B, poly_t *C)
{
    const size_t da = A->deg, db = B->deg;
    const size_t maxd = da + db;
    /* zero out target */
    for (size_t i = 0; i <= maxd; ++i) C->coeff[i] = 0;
    for (size_t i = 0; i <= da; ++i) {
        for (size_t j = 0; j <= db; ++j) {
            C->coeff[i + j] += A->coeff[i] * B->coeff[j];
        }
    }
    C->deg = maxd;
    while (C->deg > 0 && C->coeff[C->deg] == 0) --C->deg;
}

/* poly_div_monic(Num,Den,Quo)  :  Quo = Num / Den  (Den is monic, exact division) */
static void poly_div_monic(const poly_t *Num, const poly_t *Den, poly_t *Quo)
{
    const size_t ndeg = Num->deg, ddeg = Den->deg;
    const size_t qdeg = ndeg - ddeg;

    seq_t tmp[103];
    seq_t qcoeff[103];
    for (size_t i = 0; i <= ndeg; ++i) tmp[i] = Num->coeff[i];
    for (size_t i = 0; i <= qdeg; ++i) qcoeff[i] = 0;

    /* long division, from high degree downwards */
    size_t i = ndeg;
    for (;;) {
        seq_t mult = tmp[i];
        if (mult != 0) {
            qcoeff[i - ddeg] = mult;
        }
        /* subtract multiplier * Den shifted by (i - ddeg) */
        for (size_t j = 0; j <= ddeg; ++j) {
            tmp[i - ddeg + j] -= mult * Den->coeff[j];
        }
        if (i == ddeg) break;
        --i;
    }

    /* set result */
    Quo->deg = qdeg;
    while (Quo->deg > 0 && qcoeff[Quo->deg] == 0) --Quo->deg;
    for (size_t k = 0; k <= Quo->deg; ++k) Quo->coeff[k] = qcoeff[k];
}

/* compute Φ_d for each divisor d of 102 in ascending order, store in phi[]. */
static void compute_phi_102(poly_t *phi, const size_t *divs, size_t cnt)
{
    for (size_t idx = 0; idx < cnt; ++idx) {
        const size_t d = divs[idx];
        poly_t *p = &phi[idx];

        if (d == 1) {                                 /* Φ₁(x) = x‑1 */
            for (size_t i = 0; i <= 102; ++i) p->coeff[i] = 0;
            p->coeff[0] = (seq_t)-1;
            p->coeff[1] = (seq_t)1;
            p->deg = 1;
            continue;
        }

        /* numerator: x^d – 1 */
        poly_t num;
        for (size_t i = 0; i <= 102; ++i) num.coeff[i] = 0;
        num.coeff[0] = (seq_t)-1;
        num.coeff[d] = (seq_t)1;
        num.deg = d;

        /* denominator: product of Φ_e for e|d, e<d */
        poly_t denom;
        for (size_t i = 0; i <= 102; ++i) denom.coeff[i] = 0;
        denom.coeff[0] = (seq_t)1;
        denom.deg = 0;

        poly_t tmp;
        for (size_t j = 0; j < idx; ++j) {           /* proper divisors already computed */
            if (d % divs[j] == 0) {
                poly_mul(&denom, &phi[j], &tmp);
                for (size_t i = 0; i <= 102; ++i) denom.coeff[i] = tmp.coeff[i];
                denom.deg = tmp.deg;
            }
        }

        /* divide numerator by denominator */
        poly_div_monic(&num, &denom, p);
    }
}

/* Return the sequence value a(n). */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t period[102];

    if (!ready) {
        /* divisors of 102 in ascending order */
        const size_t divs[] = {1, 2, 3, 6, 17, 34, 51, 102};
        const size_t Dcnt = sizeof(divs) / sizeof(divs[0]);
        poly_t phi[Dcnt];

        compute_phi_102(phi, divs, Dcnt);

        /* Φ₁₀₂ is the last entry */
        const poly_t *phi102 = &phi[Dcnt - 1];
        const size_t deg = phi102->deg;   /* should be 32 */

        /* recurrence: a[0] = 1; a[n] = - Σ_{k=1}^{min(deg,n)} c_k * a[n-k] */
        period[0] = (seq_t)1;
        for (size_t i = 1; i < 102; ++i) {
            seq_t sum = 0;
            size_t kmax = (i <= deg) ? i : deg;
            for (size_t k = 1; k <= kmax; ++k) {
                sum += phi102->coeff[k] * period[i - k];
            }
            period[i] = -sum;
        }
        ready = true;
    }

    return period[(size_t)(n % 102)];
}
