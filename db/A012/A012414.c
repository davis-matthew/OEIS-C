/*
 * A012414  Expansion of e.g.f. arcsinh(arctan(x) * exp(x)).
 * https://oeis.org/A012414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * coeff of x^n in arcsinh(arctan(x)*exp(x))
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses power‑series manipulation and integer arithmetic.
 * It builds factorials, the arctan series, the product U=arctan(x)*exp(x),
 * the square‑root V = sqrt(1+U^2), and finally computes the series
 * R = U' / V.  The coefficient a(n) equals R_fact[n‑1] for n≥1
 * (and a(0)=0).  All arithmetic is performed with seq_t; binomial
 * coefficients are computed multiplicatively.  Caches are used so
 * repeated calls with arbitrary ordering are fast. */

#include <stdlib.h>

static idx_t cache_size = 0;

static seq_t *fact_arr       = NULL;  // factorials up to N
static seq_t *atan_fact      = NULL;  // n! * coeff of arctan(x)
static seq_t *U_fact         = NULL;  // n! * coeff of U = arctan(x)*exp(x)
static seq_t *V_fact         = NULL;  // n! * coeff of V = sqrt(1+U^2)
static seq_t *R_fact         = NULL;  // n! * coeff of R = U'/V
static seq_t *u2_fact        = NULL;  // n! * coeff of U^2

/* Make arrays large enough for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache_size > 0 && cache_size >= n + 1)
        return;

    /* allocate space for (n+1) elements of each array */
    seq_t *tmp;

    tmp = (seq_t*)realloc(fact_arr,   (n + 1) * sizeof(seq_t));
    fact_arr   = tmp;
    tmp = (seq_t*)realloc(atan_fact, (n + 1) * sizeof(seq_t));
    atan_fact  = tmp;
    tmp = (seq_t*)realloc(U_fact,    (n + 1) * sizeof(seq_t));
    U_fact     = tmp;
    tmp = (seq_t*)realloc(V_fact,    (n + 1) * sizeof(seq_t));
    V_fact     = tmp;
    tmp = (seq_t*)realloc(R_fact,    (n + 1) * sizeof(seq_t));
    R_fact     = tmp;
    tmp = (seq_t*)realloc(u2_fact,   (n + 1) * sizeof(seq_t));
    u2_fact    = tmp;

    /* compute factorials */
    fact_arr[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;

    /* arctan series: A_fact[k] = (-1)^m * (2m)! for odd k=2m+1, zero otherwise */
    for (idx_t i = 0; i <= n; ++i)
        atan_fact[i] = 0;
    for (idx_t m = 0; 2 * m + 1 <= n; ++m) {
        idx_t k = 2 * m + 1;
        seq_t val = fact_arr[2 * m];
        if (m & 1)
            val = -val;
        atan_fact[k] = val;
    }

    /* U(x) = arctan(x) * exp(x)  =>  U_fact[n] = sum_k binom(n,k) * atan_fact[k] */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;
        for (idx_t k = 0; k <= i; ++k) {
            if (k & 1)
                sum += binom * atan_fact[k];
            if (k < i) {
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        U_fact[i] = sum;
    }

    /* U^2 series: u2_fact[n] = sum_k binom(n,k) * U_fact[k] * U_fact[n-k] */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;
        for (idx_t k = 0; k <= i; ++k) {
            sum += binom * U_fact[k] * U_fact[i - k];
            if (k < i) {
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        u2_fact[i] = sum;
    }

    /* V(x) = sqrt(1 + U^2)  =>   V_fact[0] = 1, and
     *   let w_n = coeff of x^n in 1 + U^2 (w_0 = 1).  Then
     *   2*V_fact[n] = w_n! - sum_{k=1}^{n-1} binom(n,k)*V_fact[k]*V_fact[n-k]  */
    V_fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;
        for (idx_t k = 0; k <= i - 1; ++k) {
            if (k >= 1) {
                sum += binom * V_fact[k] * V_fact[i - k];
            }
            if (k < i - 1) {
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        /* w_n = u2_fact[n]  (since u2_fact[n] = n! * coeff(U^2) ) */
        V_fact[i] = (u2_fact[i] - sum) / 2;   /* division by 2 is exact */
    }

    /* R(x) = U' / V.  U' series coeffs: U'_fact[n] = U_fact[n+1] for n >= 0   */
    R_fact[0] = 1;  /* U'(0) = 1, V(0)=1 */
    for (idx_t i = 1; i <= n - 1; ++i) {   /* we need R_fact up to n-1 for maximum a(n) */
        seq_t sum = 0;
        seq_t binom = 1;
        /* compute sum_{k=1}^{n-1} binom(n,k) * V_fact[k] * R_fact[n-k] */
        for (idx_t k = 0; k <= i - 1; ++k) {
            if (k >= 1) {
                sum += binom * V_fact[k] * R_fact[i - k];
            }
            if (k < i - 1) {
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        /* A_src[n] = U_fact[n+1] */
        seq_t A_src = U_fact[i + 1];
        R_fact[i] = A_src - V_fact[i] * R_fact[0] - sum;
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    /* Ensure all data up to n are computed */
    ensure_capacity(n);
    /* a(n) = R_fact[n-1] (since R_fact[k] = k! * coeff of R' at x^k) */
    return R_fact[n - 1];
}
