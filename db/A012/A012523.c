/*
 * A012523  sech(sinh(x)*exp(x))=1-1/2!*x^2-6/3!*x^3-23/4!*x^4-20/5!*x^5...
 * https://oeis.org/A012523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n/n! in 1 / cosh(sinh(x)*exp(x))
 * The sequence is obtained by computing the exponential generating
 * function f(x)=sech(sinh(x)*exp(x)) and extracting its coefficients.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *g_coeff  = NULL;   /* g_n = 2^(n-1)  for n>=1, g_0 = 0   */
static seq_t *e_coeff  = NULL;   /* e_n  coefficient of exp(g)        */
static seq_t *e_neg    = NULL;   /* e_n  coefficient of exp(-g)       */
static seq_t *B_coeff  = NULL;   /* B_n corresponding to cosh(g)      */
static seq_t *a_coeff  = NULL;   /* a_n of the desired sequence       */
static size_t stored_n = 0;      /* maximum n for which arrays are filled */

static void compute_to(idx_t nmax)
{
    size_t i, k;
    size_t newsize = (size_t)nmax + 1;
    if (newsize > stored_n) {
        /* free old memory if any */
        free(g_coeff);
        free(e_coeff);
        free(e_neg);
        free(B_coeff);
        free(a_coeff);

        /* allocate fresh arrays */
        g_coeff  = (seq_t*)malloc(sizeof(seq_t)*newsize);
        e_coeff  = (seq_t*)malloc(sizeof(seq_t)*newsize);
        e_neg    = (seq_t*)malloc(sizeof(seq_t)*newsize);
        B_coeff  = (seq_t*)malloc(sizeof(seq_t)*newsize);
        a_coeff  = (seq_t*)malloc(sizeof(seq_t)*newsize);
        stored_n = newsize;
    }

    /* g[n] = 2^(n-1) for n>=1, g[0] = 0  */
    g_coeff[0] = 0;
    seq_t val = 1;
    for (i = 1; i <= (size_t)nmax; ++i) {
        if (i == 1) val = 1;
        else       val = val * (seq_t)2;
        g_coeff[i] = val;
    }

    /* compute e_coeff and e_neg using recurrence
       e_{m+1} = sum_{k=0}^{m} binom(m,k) * e_k * g_{m-k+1}
       e_neg_{m+1} = -sum_{k=0}^{m} binom(m,k) * e_neg_k * g_{m-k+1}
    */
    e_coeff[0] = 1;
    e_neg[0]   = 1;
    /* binomials: C[k] = binom(cur_n, k) */
    seq_t *C = (seq_t*)calloc(newsize, sizeof(seq_t));
    C[0] = 1; /* binom(0,0)=1 */

    for (size_t m = 0; m < (size_t)nmax; ++m) {
        seq_t sumE   = 0;
        seq_t sumNeg = 0;
        /* iterate k = 0..m */
        for (k = 0; k <= m; ++k) {
            seq_t term = C[k] * e_coeff[k] * g_coeff[m - k + 1];
            sumE += term;
            sumNeg += C[k] * e_neg[k] * g_coeff[m - k + 1];
        }
        e_coeff[m + 1] = sumE;
        e_neg[m + 1]   = -sumNeg;   /* sign flip */

        /* update binomial coefficients to row m+1 */
        for (k = m + 1; k >= 1; --k)
            C[k] = C[k - 1] + C[k];
        /* C[0] stays 1 */
    }
    free(C);

    /* B[n] = (e[n] + e_neg[n]) / 2  for all n */
    for (i = 0; i <= (size_t)nmax; ++i)
        B_coeff[i] = (e_coeff[i] + e_neg[i]) / (seq_t)2;

    /* compute a_coeff: inverse of B in EGF sense
       a[0] = 1
       a[n] = - sum_{k=1}^{n} binom(n,k) * B[k] * a[n-k]
    */
    a_coeff[0] = 1;
    C = (seq_t*)calloc(newsize, sizeof(seq_t));
    C[0] = 1; /* binom(0,0)=1 */

    for (k = 1; k <= (size_t)nmax; ++k) {
        /* update binom to row k */
        for (i = k; i >= 1; --i)
            C[i] = C[i - 1] + C[i];
        /* compute sum */
        seq_t sum = 0;
        for (i = 1; i <= k; ++i)
            sum += C[i] * B_coeff[i] * a_coeff[k - i];
        a_coeff[k] = -sum;
    }
    free(C);
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if ((idx_t)stored_n <= n)
        compute_to((size_t)n);
    return a_coeff[(size_t)n];
}
