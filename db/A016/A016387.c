/*
 * A016387  341st cyclotomic polynomial.
 * https://oeis.org/A016387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) are the coefficients of the 341st cyclotomic polynomial
 *            Φ_341(x), computed by the exact division Φ_341(x) = [(x^341 –
 *            1)·(x – 1)] / [(x^31 – 1)·(x^11 – 1)]. The coefficients are
 *            returned for all indices n; for n > φ(341)=300 the value is 0
 * Range:     i8 n <= 300, i16 n <= 300, i32 n <= 300, i64 n <= 300,
 *            i128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  φ(341) = 300, so we precompute 301 coefficients. */
    static seq_t *coeff = NULL;
    static const idx_t DEG = 300;
    if (coeff == NULL) {
        /* Allocate storage. */
        coeff = (seq_t*)malloc((DEG + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= DEG; ++i) coeff[i] = 0;

        /* Build numerator N(x) = (x^341 – 1)(x – 1). */
        const idx_t MAXN = 342;           /* Highest exponent of numerator. */
        seq_t *N = (seq_t*)malloc((MAXN + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= MAXN; ++i) N[i] = 0;

        /* A(x) = x^341 – 1 */
        for (idx_t e = 0; e <= 341; ++e) {
            seq_t c = (e == 0) ? -1 : ((e == 341) ? 1 : 0);
            if (c == 0) continue;
            /* multiply by (x – 1): c·x^(e+1) – c·x^e */
            N[e + 1] += c;
            N[e]     -= c;
        }

        /* Build denominator D(x) = (x^31 – 1)(x^11 – 1). */
        const idx_t MAXD = 42;           /* Highest exponent of denominator. */
        seq_t *D = (seq_t*)malloc((MAXD + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= MAXD; ++i) D[i] = 0;

        /* (x^31 – 1)(x^11 – 1) = (x^31 – 1)·(x^11 – 1)   */
        /* Compute by multiplying each term of (x^31 – 1) by (x^11 – 1). */
        /* Terms of (x^31 – 1):  -1·x^0 , +1·x^31 */
        seq_t c1 = -1; /* at exponent 0 */
        D[0 + 11] += c1;       /* -1·x^11 */
        D[0]     -= c1;        /* -(-1) = +1 */

        seq_t c2 = 1;  /* at exponent 31 */
        D[31 + 11] += c2;   /* +1·x^42 */
        D[31]      -= c2;   /* -1 */

        /* Long division: N / D gives Φ_341(x).  Result degree 300. */
        const idx_t DEGRES = DEG;        /* Result degree (0..300) */
        seq_t *Q = coeff;                /* Reuse coeff array for quotient. */
        seq_t *R = N;                    /* R is working copy of numerator */

        /* Find current highest non‑zero degree of R. */
        idx_t degR = MAXN;
        while (degR > 0 && R[degR] == 0) --degR;

        /* Leading coefficient of D is 1 (at exponent 42). */
        const idx_t LEAD_DEG = MAXD;
        seq_t lead_coeff = D[LEAD_DEG];  /* Should be 1. */

        while (degR >= LEAD_DEG) {
            /* Quotient term coefficient */
            seq_t coeff_term = R[degR] / lead_coeff;  /* exact division */
            idx_t shift = degR - LEAD_DEG;

            /* Subtract coeff_term * D(x) shifted by 'shift' from R. */
            for (idx_t j = 0; j <= LEAD_DEG; ++j) {
                R[shift + j] -= coeff_term * D[j];
            }

            /* Store quotient coefficient */
            Q[shift] += coeff_term;

            /* Update highest degree of R */
            while (degR > 0 && R[degR] == 0) --degR;
        }

        /* Free temporary arrays */
        free(D);
        free(N);

        /* All coefficients are now stored in coeff[0..300] */
    }

    if (n <= DEG) {
        return coeff[n];
    }
    /* Outside the degree of Φ_341 the coefficient is zero. */
    return 0;
}
