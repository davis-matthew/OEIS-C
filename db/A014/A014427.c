/*
 * A014427  Inverse of 418th cyclotomic polynomial.
 * https://oeis.org/A014427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of the power series expansion of 1/φ_418(x),
 *            which is periodic with period 418. The sequence is obtained by
 *            a(0) = 1 and for n ≥ 1 a(n) = −∑_{i=1}^{min(n,φ(418))} φ_i ·
 *            a(n−i) where φ_i are the coefficients of the 418‑th cyclotomic
 *            polynomial φ_418(x) = ∑_{i=0}^{φ(418)} φ_i x^i...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* constants -------------------------------------------------------------- */
#define MAX_N     418
#define MAX_DEG   420   /* φ(418) = 180, but use 420 for safety */

/* global storage for polynomials ---------------------------------------- */
static int64_t poly[MAX_N + 1][MAX_DEG + 1]; /* coefficients, LSB first */
static int    deg_poly[MAX_N + 1];           /* degree of each polynomial */
static bool   computed[MAX_N + 1] = {0};

/* forward declaration ----------------------------------------------- */
static void compute_phi(uint64_t n);

/* --------------------------------------------------------------------- */
/* compute all cyclotomic polynomials φ_d(x) for d | 418, needed for
   φ_418.  The algorithm uses the identity
        φ_n(x) = (x^n − 1) / ∏_{d|n, d<n} φ_d(x) .
   All φ_d are monic, so division is exact.                                 */
static void compute_phi(uint64_t n)
{
    if (computed[n]) return;

    /*  φ_1(x) = x − 1 ---------------------------------------------- */
    if (n == 1) {
        deg_poly[1] = 1;
        poly[1][0] = -1;
        poly[1][1] = 1;
        computed[1] = true;
        return;
    }

    /*  find all proper divisors of n --------------------------------- */
    int divs[100];
    int dcnt = 0;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i != n) { divs[dcnt++] = (int)i; }
            uint64_t j = n / i;
            if (j != i && j != n) { divs[dcnt++] = (int)j; }
        }
    }

    /*  compute proper φ_d ------------------------------------------------ */
    for (int i = 0; i < dcnt; ++i) compute_phi(divs[i]);

    /*  numerator: x^n – 1 ----------------------------------------------- */
    int64_t num[MAX_DEG + 1] = {0};
    int    deg_num = (int)n;
    if (deg_num > MAX_DEG) return; /* safety */
    num[0] = -1;
    num[deg_num] = 1;

    /*  product of proper φ_d ------------------------------------------------ */
    int64_t prod[MAX_DEG + 1] = {0};
    int    deg_prod = 0;
    prod[0] = 1;                  /* start with polynomial 1 */

    for (int i = 0; i < dcnt; ++i) {
        int d = divs[i];
        int ndig = deg_poly[d];
        int64_t tmp[MAX_DEG + 1] = {0};
        int new_deg = deg_prod + ndig;
        for (int a = 0; a <= deg_prod; ++a) {
            if (prod[a] == 0) continue;
            for (int b = 0; b <= ndig; ++b) {
                tmp[a + b] += prod[a] * poly[d][b];
            }
        }
        for (int k = 0; k <= new_deg; ++k) prod[k] = tmp[k];
        deg_prod = new_deg;
    }

    /*  divide numerator by product (monic divisor) --------------------- */
    int    deg_res = deg_num - deg_prod;
    int64_t res[MAX_DEG + 1] = {0};
    int64_t tmp[MAX_DEG + 1];
    for (int k = 0; k <= deg_num; ++k) tmp[k] = num[k];

    for (int k = deg_num; k >= deg_prod; --k) {
        int64_t coef = tmp[k];
        if (coef != 0) {
            res[k - deg_prod] = coef;            /* quotient coefficient */
            /* subtract coef * prod shifted by k - deg_prod */
            int shift = k - deg_prod;
            for (int t = 0; t <= deg_prod; ++t) {
                tmp[shift + t] -= coef * prod[t];
            }
        }
    }

    /*  store the result --------------------------------------------------- */
    deg_poly[n] = deg_res;
    for (int k = 0; k <= deg_res; ++k) poly[n][k] = res[k];
    computed[n] = true;
}

/* --------------------------------------------------------------------- */
/* main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /*  one time initialization --------------------------------------- */
    static bool init = false;
    static seq_t seq[MAX_N];          /* period length */

    if (!init) {
        init = true;

        /* compute φ_418 and all proper divisors --------------------- */
        compute_phi(MAX_N);

        /* build the periodic sequence using the recurrence */
        seq[0] = 1;   /* a(0) */
        int deg = deg_poly[MAX_N];          /* φ(418) = 180 */
        for (idx_t k = 1; k < MAX_N; ++k) {
            int64_t sum = 0;
            int up = (k < (idx_t)deg) ? (int)k : deg;
            for (int i = 1; i <= up; ++i) {
                sum += poly[MAX_N][i] * (int64_t)seq[k - i];
            }
            seq[k] = -(seq_t)sum;   /* φ_0 = 1 */
        }
    }

    return seq[n % MAX_N];
}
