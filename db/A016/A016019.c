/*
 * A016019  Inverse of 2010th cyclotomic polynomial.
 * https://oeis.org/A016019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the formal power series *
 *            1/Φ_2010(x), where Φ_2010 is the 2010‑th cyclotomic polynomial.
 *            * The sequence is periodic with period 2010
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper structure for polynomials */
typedef struct {
    size_t deg;
    seq_t *coeff;          /* coeff[i] is coefficient of x^i, 0 <= i <= deg */
} Poly;

/* Multiply two polynomials. Returns a newly allocated Poly. */
static Poly poly_mul(const seq_t *a, size_t da, const seq_t *b, size_t db) {
    size_t resd = da + db;
    seq_t *res = (seq_t *)calloc(resd + 1, sizeof(seq_t));
    for (size_t i = 0; i <= da; ++i) {
        for (size_t j = 0; j <= db; ++j) {
            res[i + j] += a[i] * b[j];
        }
    }
    return (Poly){resd, res};
}

/* Divide polynomial A (degree dA) by monic polynomial B (degree dB).
 * It assumes B divides A exactly. Returns the quotient polynomial Q. */
static Poly poly_div_monic(const seq_t *A, size_t dA, const seq_t *B, size_t dB) {
    size_t dQ = dA - dB;
    seq_t *Q = (seq_t *)calloc(dQ + 1, sizeof(seq_t));
    seq_t *cur = (seq_t *)malloc((dA + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= dA; ++i) cur[i] = A[i];

    for (int i = (int)dA; i >= (int)dB; --i) {
        seq_t coeff = cur[i];
        if (coeff != 0) {
            Q[i - dB] = coeff;
            for (size_t j = 0; j <= dB; ++j) {
                cur[i - dB + j] -= coeff * B[j];
            }
        }
    }
    free(cur);

    /* Trim leading zeros (if any) */
    while (dQ > 0 && Q[dQ] == 0) --dQ;
    return (Poly){dQ, Q};
}

/* Compute and cache the period of the sequence. */
static void compute_period(seq_t period[2010]) {
    const idx_t PERIOD = 2010;
    /* Find all divisors of PERIOD */
    idx_t divs[32];
    size_t divcnt = 0;
    for (idx_t i = 1; i <= PERIOD; ++i)
        if (PERIOD % i == 0)
            divs[divcnt++] = i;
    /* sort divisors (bubble sort is fine for <=16 elements) */
    for (size_t i = 0; i < divcnt; ++i)
        for (size_t j = i + 1; j < divcnt; ++j)
            if (divs[i] > divs[j]) {
                idx_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    /* Allocate polynomials for each divisor */
    Poly *polys = (Poly *)calloc(divcnt, sizeof(Poly));

    for (size_t idx = 0; idx < divcnt; ++idx) {
        idx_t d = divs[idx];
        if (d == 1) {
            /* Φ_1(x) = x - 1 */
            polys[idx].deg = 1;
            polys[idx].coeff = (seq_t *)malloc(2 * sizeof(seq_t));
            polys[idx].coeff[0] = -1;
            polys[idx].coeff[1] = 1;
            continue;
        }

        /* Polynomial A = x^d - 1 */
        size_t da = d;
        seq_t *Acoeff = (seq_t *)calloc(da + 1, sizeof(seq_t));
        Acoeff[0] = -1;
        Acoeff[da] = 1;

        /* Product of Φ_e for all proper divisors e of d */
        Poly prod;
        prod.deg = 0;
        prod.coeff = (seq_t *)malloc(sizeof(seq_t));
        prod.coeff[0] = 1;    /* polynomial "1" */

        for (size_t j = 0; j < idx; ++j) {
            if (d % divs[j] == 0) {
                Poly tmp = poly_mul(prod.coeff, prod.deg,
                                    polys[j].coeff, polys[j].deg);
                free(prod.coeff);
                prod.coeff = tmp.coeff;
                prod.deg   = tmp.deg;
            }
        }

        /* Divide A by the product to obtain Φ_d */
        Poly q = poly_div_monic(Acoeff, da, prod.coeff, prod.deg);
        polys[idx].coeff = q.coeff;
        polys[idx].deg   = q.deg;

        /* Clean up */
        free(Acoeff);
        free(prod.coeff);
    }

    /* Find index of PERIOD in the divisor list */
    size_t period_index = 0;
    for (size_t i = 0; i < divcnt; ++i)
        if (divs[i] == PERIOD) {
            period_index = i;
            break;
        }

    /* Compute the periodic part using the recurrence */
    const size_t phdeg = polys[period_index].deg;
    const seq_t *ph = polys[period_index].coeff;
    period[0] = 1;
    for (size_t k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        size_t imax = (phdeg < k) ? phdeg : k;
        for (size_t i = 1; i <= imax; ++i) {
            sum += ph[i] * period[k - i];
        }
        period[k] = -sum;
    }

    /* Free allocated polynomials */
    for (size_t i = 0; i < divcnt; ++i)
        free(polys[i].coeff);
    free(polys);
}

/* Main sequence function */
seq_t a(idx_t n) {
    static idx_t first = 1;
    static seq_t period[2010];
    static bool initialized = false;
    if (!initialized) {
        compute_period(period);
        initialized = true;
    }
    return period[(size_t)(n % 2010)];
}
