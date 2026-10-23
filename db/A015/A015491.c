/*
 * A015491  Inverse of 1482nd cyclotomic polynomial.
 * https://oeis.org/A015491
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

/*  A015491 – Inverse of 1482nd cyclotomic polynomial
 *  a(0) = 1
 *  a(n) = - Σ_{k=1}^{φ(1482)} c_k · a(n-k)  (n ≥ 1)
 *  where Φ_1482(x) = Σ_{k=0}^{φ(1482)} c_k x^k is the 1482nd cyclotomic
 *  polynomial and the sequence is periodic with period 1482.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- configuration ------------------------------------------------- */
#define N       1482                  /* cyclotomic index                     */
#define PERIOD  1482                  /* period of the sequence               */
#define MAXD    1485                  /* >= N+1 to hold dividend x^N-1         */

/* ---------- auxiliary polynomial arithmetic ----------------------------- */
static void poly_mul(const seq_t *a, size_t da,
                     const seq_t *b, size_t db,
                     seq_t *res)          /* res must be zeroed beforehand */
{
    for (size_t i = 0; i <= da; ++i)
        for (size_t j = 0; j <= db; ++j)
            res[i + j] += a[i] * b[j];
}

/* Divide dividend by divisor (both stored ascending).
 * Assumes divisor divides dividend exactly.
 * quotient must be zeroed before call. */
static void poly_divide(const seq_t *dividend, size_t dd,
                        const seq_t *divisor, size_t dv,
                        seq_t *quo)
{
    static seq_t tmp[2 * MAXD];
    for (size_t i = 0; i <= dd; ++i) tmp[i] = dividend[i];
    if (dd > 2 * MAXD) return; /* safety, should not happen */

    for (int i = (int)dd; i >= (int)dv; --i) {
        seq_t coef = tmp[i];
        if (!coef) continue;
        quo[i - dv] = coef;
        for (size_t j = 0; j <= dv; ++j)
            tmp[i - dv + j] -= coef * divisor[j];
    }
    /* remainder is zero by construction */
}

/* ---------- internal storage -------------------------------------------- */
static const seq_t (*phi)[MAXD] = NULL; /* array of Φ_d polynomials */
static const size_t *phi_deg = NULL;    /* corresponding degrees      */
static size_t _phi_nd = 0;
static const seq_t *phi_1482 = NULL;    /* Φ_1482 coefficients        */
static size_t deg_1482 = 0;             /* degree of Φ_1482           */

/* cache of the first PERIOD terms                        */
static seq_t a_cache[PERIOD];
static size_t a_cache_size = 0;

/* ---------- initialization function ------------------------------------- */
static void init_sequence(void)
{
    if (a_cache_size != 0) return;   /* already done */

    /* 1. Enumerate all divisors of N (2*3*13*19) in ascending order   */
    size_t divs[16];
    size_t nd = 0;
    for (size_t a = 0; a <= 1; ++a)
        for (size_t b = 0; b <= 1; ++b)
            for (size_t c = 0; c <= 1; ++c)
                for (size_t d = 0; d <= 1; ++d) {
                    size_t v = 1;
                    if (a) v *= 2;
                    if (b) v *= 3;
                    if (c) v *= 13;
                    if (d) v *= 19;
                    divs[nd++] = v;
                }
    /* sort */
    for (size_t i = 0; i + 1 < nd; ++i)
        for (size_t j = i + 1; j < nd; ++j)
            if (divs[i] > divs[j]) {
                size_t t = divs[i]; divs[i] = divs[j]; divs[j] = t;
            }

    /* 2. Allocate static arrays for all divisor polynomials           */
    static seq_t _phi[16][MAXD];
    static size_t _phi_deg[16];
    phi = _phi;
    phi_deg = _phi_deg;
    _phi_nd = nd;

    /* temporary arrays for intermediate computations                */
    static seq_t tmpProd[MAXD];
    static seq_t tmpRes[MAXD];
    static seq_t dividend[MAXD];
    static seq_t quotient[MAXD];

    for (size_t i = 0; i < nd; ++i) {
        size_t d = divs[i];
        if (d == 1) { /* Φ_1(x) = x - 1 */
            _phi_deg[i] = 1;
            _phi[i][0] = -1;
            _phi[i][1] = 1;
            continue;
        }

        /* product of Φ_t over proper divisors t | d               */
        for (size_t k = 0; k < MAXD; ++k) tmpProd[k] = 0;
        tmpProd[0] = 1;
        size_t degProd = 0;

        for (size_t j = 0; j < i; ++j) {
            size_t t = divs[j];
            if (d % t != 0) continue;
            /* multiply tmpProd by Φ_t */
            for (size_t k = 0; k < MAXD; ++k) tmpRes[k] = 0;
            poly_mul(tmpProd, degProd, _phi[j], _phi_deg[j], tmpRes);
            for (size_t k = 0; k < MAXD; ++k) tmpProd[k] = tmpRes[k];
            degProd += _phi_deg[j];
        }

        /* dividend = x^d - 1 */
        for (size_t k = 0; k < MAXD; ++k) dividend[k] = 0;
        dividend[0] = -1;
        if (d < MAXD) dividend[d] = 1;
        size_t degDiv = d;

        /* divide dividend by tmpProd to get Φ_d */
        for (size_t k = 0; k < MAXD; ++k) quotient[k] = 0;
        poly_divide(dividend, degDiv, tmpProd, degProd, quotient);

        _phi_deg[i] = degDiv - degProd;
        for (size_t k = 0; k <= _phi_deg[i]; ++k)
            _phi[i][k] = quotient[k];

        if (d == N) {
            phi_1482 = _phi[i];
            deg_1482 = _phi_deg[i];
        }
    }

    /* 3. Compute the sequence for one full period                   */
    a_cache[0] = 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= deg_1482 && k <= n; ++k)
            sum += phi_1482[k] * a_cache[n - k];
        a_cache[n] = -sum;
    }
    a_cache_size = PERIOD;
}

/* ---------- public function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (a_cache_size == 0) init_sequence();
    return a_cache[(size_t)n % PERIOD];
}
