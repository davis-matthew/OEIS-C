/*
 * A015947  Inverse of 1938th cyclotomic polynomial.
 * https://oeis.org/A015947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015947 – inverse of the 1938‑th cyclotomic polynomial
 * Formula: a(0) = 1,  a(n) = -∑_{k=1}^{min(n,φ(1938))} c_k · a(n−k)
 * where c_k (k ≥ 0) are the coefficients of Φ_1938(x) (Φ_1938(x) = ∑_{k=0}^{φ(1938)} c_k x^k).
 * The sequence is periodic with period 1938.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* period of the sequence */
#define PERIOD 1938

/* number of divisors of 1938 (including 1 and 1938). 1938 = 2·3·17·19 has 16 divisors. */
#define MAXDIV 20

/* number of divisors up. */
static size_t _num_divisors(void);

/* division of two monic integer polynomials, exact division */
static void poly_div_exact(const seq_t *num, size_t len_num,
                           const seq_t *den, size_t len_den,
                           seq_t *quot);

/* ------------------------------------------------------------------ */
/* compute the coefficients of Φ_1938(x) (size 577 because φ(1938)=576) */
static const seq_t *compute_phi1938_coeffs(void);

/* ------------------------------------------------------------------ */
/* main sequence function */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t seq[PERIOD];          /* cached periodic values */
    static const seq_t *phi_coeffs;    /* coefficients of Φ_1938(x) */

    if (!ready) {
        /* 1. compute Φ_1938 coefficients only once */
        phi_coeffs = compute_phi1938_coeffs();

        /* 2. compute a[0..PERIOD-1] by the recurrence */
        seq[0] = 1;
        const size_t deg = 576;          /* φ(1938) */
        for (size_t k = 1; k < PERIOD; ++k) {
            seq_t sum = 0;
            size_t limit = (k < deg) ? k : deg;
            for (size_t i = 1; i <= limit; ++i) {
                sum += phi_coeffs[i] * seq[k - i];
            }
            seq[k] = -sum;
        }
        ready = true;
    }

    /* 3. return periodic value */
    return seq[(size_t)(n % PERIOD)];
}

/* ------------------------------------------------------------------ */
/* helper: list divisors of 1938, sorted ascending */
static size_t _num_divisors(void)
{
    static size_t divs[MAXDIV];
    size_t cnt = 0;
    const idx_t N = 1938;
    for (idx_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            divs[cnt++] = d;
            if (d != N / d)
                divs[cnt++] = N / d;
        }
    }
    /* simple bubble sort */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (divs[i] > divs[j]) {
                idx_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
    return cnt;
}

/* ------------------------------------------------------------------ */
/* exact polynomial division: num / den => quot, assume den monic and division exact */
static void poly_div_exact(const seq_t *num, size_t len_num,
                           const seq_t *den, size_t len_den,
                           seq_t *quot)
{
    /* work on a copy of num */
    seq_t *work = (seq_t *)malloc(len_num * sizeof(seq_t));
    memcpy(work, num, len_num * sizeof(seq_t));

    signed long long deg_num = (long long)len_num - 1;
    signed long long deg_den = (long long)len_den - 1;
    signed long long deg_quot = deg_num - deg_den;

    for (signed long long i = deg_num; i >= deg_den; --i) {
        seq_t coeff = work[i];
        quot[i - deg_den] = coeff;
        for (size_t j = 0; j <= deg_den; ++j) {
            work[i - deg_den + j] -= coeff * den[j];
        }
    }
    free(work);
    /* lower part of quot remains zero because quot is calloced by caller */
}

/* ------------------------------------------------------------------ */
/* compute coefficients of Φ_1938(x) */
static const seq_t *compute_phi1938_coeffs(void)
{
    /* list divisors of 1938 */
    const size_t divcnt = _num_divisors();
    idx_t divs[MAXDIV];
    /* fill again to use later */
    idx_t N = 1938;
    size_t idx = 0;
    for (idx_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            divs[idx++] = d;
            if (d != N / d) divs[idx++] = N / d;
        }
    }
    /* ensure sorted */
    for (size_t i = 0; i < divcnt; ++i)
        for (size_t j = i + 1; j < divcnt; ++j)
            if (divs[i] > divs[j]) {
                idx_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }

    /* data structure for each divisor's polynomial */
    struct {
        idx_t d;
        seq_t *coeff;
        size_t len;
    } polys[MAXDIV];

    for (size_t i = 0; i < divcnt; ++i) {
        polys[i].d = divs[i];
        /* start with x^d – 1 */
        polys[i].len = (size_t)polys[i].d + 1;
        polys[i].coeff = (seq_t *)calloc(polys[i].len, sizeof(seq_t));
        polys[i].coeff[0] = -1;
        polys[i].coeff[polys[i].len - 1] = 1;
    }

    /* iterate over divisors in ascending order, divide by all proper divisors */
    for (size_t i = 0; i < divcnt; ++i) {
        if (polys[i].d == 1) /* Φ_1 = x – 1 already correct */
            continue;
        for (size_t j = 0; j < i; ++j) {
            if (polys[i].d % polys[j].d == 0) {
                size_t len_new = polys[i].len - polys[j].len + 1;
                seq_t *quot = (seq_t *)calloc(len_new, sizeof(seq_t));
                poly_div_exact(polys[i].coeff, polys[i].len,
                               polys[j].coeff, polys[j].len,
                               quot);
                free(polys[i].coeff);
                polys[i].coeff = quot;
                polys[i].len = len_new;
            }
        }
    }

    /* find Φ_1938 */
    seq_t *phi_coeff = NULL;
    for (size_t i = 0; i < divcnt; ++i) {
        if (polys[i].d == N) {
            phi_coeff = polys[i].coeff;
            break;
        }
    }

    /* free all other polynomials */
    for (size_t i = 0; i < divcnt; ++i) {
        if (polys[i].coeff != phi_coeff)
            free(polys[i].coeff);
    }

    /* phi_coeff now holds the coefficients of Φ_1938(x) */
    return phi_coeff;
}
