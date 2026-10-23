/*
 * A016389  350th cyclotomic polynomial.
 * https://oeis.org/A016389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficient of the 350th cyclotomic polynomial Φ_350(x):
   a(n) = coefficient of x^n in Φ_350(x) (n ≥ 0). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *poly_mul(const seq_t *a, size_t da, const seq_t *b, size_t db, size_t *deg_out)
{
    size_t d = da + db;
    seq_t *res = alloc_seq_array(d + 1);   /* zero‑initialised */
    for (size_t i = 0; i <= d; ++i) res[i] = 0;
    for (size_t i = 0; i <= da; ++i) {
        for (size_t j = 0; j <= db; ++j) {
            res[i + j] += a[i] * b[j];
        }
    }
    *deg_out = d;
    return res;
}

static seq_t *poly_div(const seq_t *P, size_t degP, const seq_t *D, size_t degD, size_t *degQ_out)
{
    size_t degQ = degP - degD;
    seq_t *Q = alloc_seq_array(degQ + 1);
    seq_t *rem = alloc_seq_array(degP + 1);
    for (size_t i = 0; i <= degP; ++i) rem[i] = P[i];

    for (size_t i = degP; ; --i) {
        seq_t coeff = rem[i];
        Q[i - degD] = coeff;
        if (coeff != 0) {
            for (size_t j = 0; j <= degD; ++j) {
                rem[i - degD + j] -= coeff * D[j];
            }
        }
        if (i == degD) break;
    }
    free_seq_array(rem);
    *degQ_out = degQ;
    return Q;
}

static seq_t *coeffs_global[12];
static size_t degrees_global[12];
static bool initialized_global = false;

static void init_coeffs(void)
{
    static const idx_t divisors[12] = {
        1, 2, 5, 7, 10, 14, 25, 35, 50, 70, 175, 350
    };
    const size_t div_cnt = 12;

    for (size_t idx = 0; idx < div_cnt; ++idx) {
        idx_t d = divisors[idx];

        /* Build D(x) = product of Φ_e(x) for e|d, e<d */
        size_t degD = 0;
        seq_t *D = alloc_seq_array(1);   /* polynomial '1' */
        D[0] = 1;

        for (size_t j = 0; j < idx; ++j) {
            idx_t e = divisors[j];
            if (d % e == 0) {
                size_t new_deg;
                seq_t *newD = poly_mul(D, degD, coeffs_global[j], degrees_global[j], &new_deg);
                free_seq_array(D);
                D = newD;
                degD = new_deg;
            }
        }

        /* P(x) = x^d - 1 */
        size_t degP = (size_t)d;
        seq_t *P = alloc_seq_array(degP + 1);
        for (size_t i = 0; i <= degP; ++i) P[i] = 0;
        P[0]  = (seq_t)-1;
        P[degP] = 1;

        /* Φ_d(x) = P(x) / D(x)  (exact division) */
        size_t degQ;
        seq_t *Q = poly_div(P, degP, D, degD, &degQ);

        /* Store result for future use */
        coeffs_global[idx] = Q;
        degrees_global[idx] = degQ;

        /* Clean up temporaries */
        free_seq_array(D);
        free_seq_array(P);
    }

    initialized_global = true;
}

seq_t a(idx_t n)
{
    if (!initialized_global) init_coeffs();

    const size_t max_idx = 11;   /* Φ_350 is the last divisor */
    if ((size_t)n <= degrees_global[max_idx]) {
        return coeffs_global[max_idx][(size_t)n];
    }
    return 0;
}
