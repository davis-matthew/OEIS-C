/*
 * A230003  Array of coefficients of numerator polynomials of the rational
 *          function p(n, x + 1/x), where p(n,x) is the n-th cyclotomic
 *          polynomial.
 * https://oeis.org/A230003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A230003:
   a(n) is the n‑th coefficient of the flattened list of numerators
   of the rational functions p(n, x + 1/x) where p(n,x) is the
   n‑th cyclotomic polynomial.
   The code constructs the cyclotomic polynomials recursively,
   evaluates them at x + 1/x, clears denominators, and returns
   the requested coefficient. */

#include <stdlib.h>
#include <stdint.h>

/* Helper for allocating sequences that works in C and C++ */
static inline seq_t *alloc_seq(size_t n)
{
#ifndef __cplusplus
    return (seq_t *)malloc(n * sizeof(seq_t));
#else
    return new seq_t[n];
#endif
}

static inline void free_seq_arr(seq_t *p)
{
#ifndef __cplusplus
    free(p);
#else
    delete[] p;
#endif
}

/* ---------- Data structures ---------- */
typedef struct {
    size_t len;   /* number of coefficients (degree+1) */
    seq_t *coeff; /* coefficient array, ascending degree */
} Poly;

/* ---------- Global state ---------- */
static idx_t *phi_vals      = NULL;   /* Euler's totient, indexed by n */
static Poly *phi_polys     = NULL;   /* Cyclotomic polynomial for each n */
static idx_t *cum_len      = NULL;   /* cumulative lengths, cum_len[0]=0 */
static size_t phi_cap      = 0;      /* capacity of phi arrays */
static size_t cum_cap      = 0;      /* capacity of cum_len array */
static size_t rows_done    = 0;      /* number of rows already computed */
static idx_t  next_row     = 1;      /* next row index to compute */

/* ---------- Utility: resize global arrays ---------- */
static void ensure_phi_capacity(idx_t n)
{
    if ((size_t)n >= phi_cap) {
        size_t new_cap = phi_cap ? phi_cap * 2 : 8;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        phi_vals  = (idx_t *)realloc(phi_vals,  new_cap * sizeof(idx_t));
        phi_polys = (Poly *)realloc(phi_polys, new_cap * sizeof(Poly));
        for (size_t i = phi_cap; i < new_cap; ++i) {
            phi_vals[i]  = 0;
            phi_polys[i].len   = 0;
            phi_polys[i].coeff = NULL;
        }
        phi_cap = new_cap;
    }
}

static void ensure_cum_capacity()
{
    if (rows_done + 1 > cum_cap) {
        size_t new_cap = cum_cap ? cum_cap * 2 : 8;
        while (rows_done + 1 > new_cap) new_cap <<= 1;
        cum_len = (idx_t *)realloc(cum_len, new_cap * sizeof(idx_t));
        cum_cap = new_cap;
    }
}

/* ---------- Compute Euler's totient for n ---------- */
static idx_t get_phi_val(idx_t n)
{
    if (n < phi_cap && phi_vals[n] != 0) return phi_vals[n];
    idx_t result = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    if (n < phi_cap) phi_vals[n] = result;
    return result;
}

/* ---------- Multiply two polynomials (monic or not) ---------- */
static Poly poly_mul(const Poly *a, const Poly *b)
{
    Poly r;
    r.len = a->len + b->len - 1;
    r.coeff = alloc_seq(r.len);
    for (size_t i = 0; i < r.len; ++i) r.coeff[i] = 0;
    for (size_t i = 0; i < a->len; ++i) {
        for (size_t j = 0; j < b->len; ++j) {
            r.coeff[i + j] += a->coeff[i] * b->coeff[j];
        }
    }
    return r;
}

/* ---------- Divide dividend by monic divisor, return quotient ---------- */
static Poly poly_div(const Poly *dividend, const Poly *divisor)
{
    size_t degN = dividend->len - 1;
    size_t degD = divisor->len  - 1;
    size_t qdeg = degN - degD;
    Poly quotient;
    quotient.len = qdeg + 1;
    quotient.coeff = alloc_seq(quotient.len);
    for (size_t i = 0; i < quotient.len; ++i) quotient.coeff[i] = 0;

    /* Make a mutable copy of dividend */
    seq_t *rem = alloc_seq(dividend->len);
    for (size_t i = 0; i < dividend->len; ++i) rem[i] = dividend->coeff[i];

    /* Do long division from highest degree downwards */
    for (int k = (int)qdeg; k >= 0; --k) {
        seq_t coef = rem[degD + (size_t)k];
        quotient.coeff[(size_t)k] = coef;
        if (coef == 0) continue;
        for (size_t i = 0; i <= degD; ++i) {
            rem[i + (size_t)k] -= coef * divisor->coeff[i];
        }
    }
    free_seq_arr(rem);
    return quotient;
}

/* ---------- Recursive construction of Φ_n(t) ---------- */
static Poly* get_phi_poly(idx_t n)
{
    if (n < phi_cap && phi_polys[n].len != 0) return &phi_polys[n];
    ensure_phi_capacity(n);

    if (n == 1) { /* Φ₁(t) = t - 1 */
        Poly *P = &phi_polys[n];
        P->len = 2;
        P->coeff = alloc_seq(2);
        P->coeff[0] = -1;
        P->coeff[1] = 1;
        return P;
    }

    /* Compute product of proper divisors' cyclotomic polynomials */
    Poly prod;
    prod.len = 1;
    prod.coeff = alloc_seq(1);
    prod.coeff[0] = 1;

    idx_t m = n;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t d1 = d, d2 = m / d;
            if (d1 < n && d1 >= 1) {
                Poly *pd = get_phi_poly(d1);
                Poly tmp = poly_mul(&prod, pd);
                free_seq_arr(prod.coeff);
                prod = tmp;
            }
            if (d2 != d1 && d2 < n && d2 >= 1) {
                Poly *pd = get_phi_poly(d2);
                Poly tmp = poly_mul(&prod, pd);
                free_seq_arr(prod.coeff);
                prod = tmp;
            }
        }
    }

    /* Build dividend polynomial t^n - 1, represented as x^n - 1 after clearing denominators */
    Poly dividend;
    dividend.len = n + 1;
    dividend.coeff = alloc_seq(dividend.len);
    for (size_t i = 0; i < dividend.len; ++i) dividend.coeff[i] = 0;
    dividend.coeff[0] = -1;
    dividend.coeff[n] = 1;

    /* Divide to obtain Φ_n(t) */
    Poly quotient = poly_div(&dividend, &prod);
    free_seq_arr(dividend.coeff);
    free_seq_arr(prod.coeff);

    /* Store result */
    phi_polys[n].len   = quotient.len;
    phi_polys[n].coeff = quotient.coeff;
    return &phi_polys[n];
}

/* ---------- Initialization ---------- */
static void init(void)
{
    if (phi_cap != 0) return;        /* already initialized */
    ensure_phi_capacity(16);
    cum_cap = 8;
    cum_len = (idx_t *)calloc(cum_cap, sizeof(idx_t));

    /* Row 0: single coefficient 1 */
    cum_len[0] = 0;
    cum_len[1] = 1;
    rows_done = 1;   /* rows 0 .. 0 computed */
    next_row  = 1;   /* next row to compute is 1 */
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    init();

    /* Extend rows until cumulative length > n */
    while (cum_len[rows_done] <= n) {
        ensure_phi_capacity(next_row);

        /* Compute φ(next_row) if needed */
        idx_t phi_val = get_phi_val(next_row);

        /* Build cyclotomic polynomial Φ_{next_row} */
        get_phi_poly(next_row);

        size_t row_len = (next_row == 0) ? 1 : (size_t)(2 * phi_val + 1);
        ensure_cum_capacity();
        cum_len[rows_done + 1] = cum_len[rows_done] + row_len;

        ++rows_done;
        ++next_row;
    }

    /* Locate the row containing n */
    idx_t r = 0;
    while (r + 1 < rows_done && cum_len[r + 1] <= n) ++r;
    idx_t offset = n - cum_len[r];

    if (r == 0) return (seq_t)1;   /* only coefficient for row 0 */

    /* Retrieve Φ_r(t) coefficients */
    Poly *P = get_phi_poly(r);
    idx_t d = get_phi_val(r);            /* φ(r) = degree of Φ_r(t) */
    size_t deg_target = (size_t)(2 * d - offset);  /* degree of desired coefficient */

    seq_t result = 0;
    for (size_t j = 0; j <= d; ++j) {
        seq_t aj = P->coeff[j];
        seq_t binom = 1;                 /* C(j,0) */
        for (size_t i = 0; i <= j; ++i) {
            size_t k = (size_t)d + j - 2 * i;
            if (k == deg_target) {
                result += aj * binom;
            }
            if (i < j) {
                binom = (binom * (seq_t)(j - i)) / (seq_t)(i + 1);
            }
        }
    }
    return result;
}
