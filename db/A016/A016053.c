/*
 * A016053  Inverse of 2044th cyclotomic polynomial.
 * https://oeis.org/A016053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Validated: standard, pass, 97 terms through n = 2987 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of n-th term in the power series of the reciprocal
 * of the 2044‑th cyclotomic polynomial Φ_2044(x).  The resulting sequence is
 * periodic with period 2044.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Divide monic polynomial num by monic polynomial den (both represented
 * by coefficient arrays).  The quotient is returned; the caller owns it.
 * No memory is freed here. */
static seq_t *divide_by_monic(const seq_t *num, int deg_num,
                              const seq_t *den, int deg_den)
{
    int deg_q = deg_num - deg_den;
    seq_t *rem = mem_new(seq_t, (size_t)(deg_num + 1));
    mem_copy(rem, num, (size_t)(deg_num + 1) * sizeof(seq_t));
    seq_t *quot = mem_new(seq_t, (size_t)(deg_q + 1));

    for (int i = deg_num; i >= deg_den; --i) {
        seq_t coef = rem[i];
        quot[i - deg_den] = coef;
        for (int j = 0; j <= deg_den; ++j)
            rem[i - deg_den + j] -= coef * den[j];
    }
    mem_free(rem);
    return quot;
}

/* The types seq_t and idx_t are provided by the build environment; do not
 * define them here.  The function declared in the prelude must match this
 * signature exactly. */
static bool ready = false;

/* Polynomial coefficients of Φ_d(x) for all divisors d of 2044.  Each array
 * is allocated once during initialization.  The arrays are indexed by the
 * exponent of x, from 0 up to the degree of the polynomial. */
static seq_t *phi_arr[2045] = {0};
static int    deg_phi_arr[2045] = {0};

/* Array holding the first 2044 coefficients of the inverse series of Φ_2044(x). */
static seq_t inv_arr[2044];

/* Multiply two polynomials (used only during cyclotomic construction). */
static seq_t *poly_mul(const seq_t *A, int da, const seq_t *B, int db)
{
    int deg = da + db;
    seq_t *res = mem_new(seq_t, (size_t)(deg + 1));
    for (int i = 0; i <= da; ++i)
        for (int j = 0; j <= db; ++j)
            res[i + j] += A[i] * B[j];
    return res;
}

/* One‑time preparation: compute all cyclotomic polynomials Φ_d(x)
 * for d | 2044 and then the first 2044 coefficients of the inverse series. */
static void init_sequence(void)
{
    if (ready) return;

    /* Divisors of 2044, in ascending order. */
    static const idx_t divs[] = {1, 2, 4, 7, 14, 28, 73, 146, 292, 511, 1022, 2044};
    const size_t ndiv = sizeof(divs) / sizeof(divs[0]);

    /* Φ_1(x) = x – 1. */
    seq_t *phi1 = mem_new(seq_t, 2);
    phi1[0] = -1;
    phi1[1] = 1;
    phi_arr[1] = phi1;
    deg_phi_arr[1] = 1;

    /* Compute Φ_d(x) for each divisor d > 1. */
    for (size_t i = 1; i < ndiv; ++i) {
        idx_t d = divs[i];
        if (d == 1) continue;

        /* x^d – 1. */
        int deg_num = (int)d;
        seq_t *num = mem_new(seq_t, (size_t)(deg_num + 1));
        num[0] = -1;
        num[deg_num] = 1;

        /* Divide out Φ_e(x) for every proper divisor e of d. */
        for (size_t j = 0; j < i; ++j) {
            idx_t e = divs[j];
            if (d % e == 0) {
                int deg_e = deg_phi_arr[e];
                seq_t *q = divide_by_monic(num, deg_num, phi_arr[e], deg_e);
                mem_free(num);
                num = q;
                deg_num -= deg_e;
            }
        }

        phi_arr[d] = num;
        deg_phi_arr[d] = deg_num;
    }

    /* Compute the inverse series a(0)…a(2043); Φ_2044 has degree φ_deg. */
    seq_t *phi = phi_arr[2044];
    int phi_deg = deg_phi_arr[2044];

    inv_arr[0] = 1;
    for (int n = 1; n < 2044; ++n) {
        seq_t sum = 0;
        int maxk = (n < phi_deg) ? n : phi_deg;
        for (int k = 1; k <= maxk; ++k)
            sum += phi[k] * inv_arr[n - k];
        inv_arr[n] = -sum;
    }

    ready = true;
}

/* Public interface required by the test harness. */
seq_t a(idx_t n)
{
    if (!ready) init_sequence();
    idx_t r = n % (idx_t)2044;
    return inv_arr[(size_t)r];
}
