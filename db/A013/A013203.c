/*
 * A013203  Expansion of e.g.f. log(sech(x) + arcsin(x)).
 * https://oeis.org/A013203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coeff of x^n in e.g.f. log(sech(x)+arcsin(x)), multiplied
 *            by n!. Recurrence: a(0) = 0 a(1) = 1 for n >= 1: a(n+1) = t[n+1]
 *            - Σ_{j=1}^{n} t[n-j+1] * C(n, j-1) * a[j] where t[k] = k! *
 *            coeff of x^k in (sech(x)+arcsin(x)): t[0] = 1, t[2m+1] =
 *            (2m-1)^2 * t[2m-1] (odd indices) t[2m] = EulerNumb...
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper macros for memory allocation that work in C and C++ */
#ifdef __cplusplus
#define NEW_ARRAY(type, n)   new type[(n)]
#define DELETE_ARRAY(ptr)    delete [] (ptr)
#else
#define NEW_ARRAY(type, n)   ((type*) malloc(sizeof(type)*(n)))
#define DELETE_ARRAY(ptr)    free(ptr)
#endif

static idx_t    g_max_n = 0;
static seq_t *  g_a   = NULL;  /* a[0..g_max_n] */
static seq_t *  g_t   = NULL;  /* t[0..g_max_n] */

static void compute_t_array(idx_t N)
{
    /* allocate t[0..N] */
    g_t = NEW_ARRAY(seq_t, N + 1);
    g_t[0] = (seq_t)1;
    if (N == 0) return;

    /* odd indices: t[1] = 1 */
    g_t[1] = (seq_t)1;
    for (idx_t m = 1; 2*m+1 <= N; ++m) {
        idx_t idx  = 2*m + 1;
        idx_t prev = idx - 2;
        seq_t mult = (seq_t)(2*m - 1);
        mult = mult * mult;            /* (2m-1)^2 */
        g_t[idx] = g_t[prev] * mult;
    }

    /* even indices via Euler numbers */
    /* Euler numbers are stored in t[2m] itself */
    g_t[0] = (seq_t)1;   /* Euler number E0 = 1 */

    for (idx_t m = 1; 2*m <= N; ++m) {
        /* compute E_{2m} = - Σ_{k=0}^{m-1} C(2m,2k) * E_{2k} */
        seq_t sum  = (seq_t)0;
        seq_t comb = (seq_t)1;   /* C(2m,0) */
        for (idx_t k = 0; k <= m-1; ++k) {
            sum += comb * g_t[2*k];
            if (k < m-1) {
                seq_t num1 = (seq_t)(2*m - 2*k);
                seq_t num2 = (seq_t)(2*m - 2*k - 1);
                seq_t den1 = (seq_t)(2*k + 2);
                seq_t den2 = (seq_t)(2*k + 1);
                comb = (comb * num1 * num2) / (den1 * den2);
            }
        }
        g_t[2*m] = -sum;
    }
}

static void compute_a_array(idx_t N)
{
    /* allocate a[0..N] */
    g_a = NEW_ARRAY(seq_t, N + 1);
    g_a[0] = (seq_t)0;
    if (N == 0) return;
    g_a[1] = (seq_t)1;

    for (idx_t n = 1; n <= N-1; ++n) {
        seq_t bi  = g_t[n+1];
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;     /* C(n,0) */
        for (idx_t j = 1; j <= n; ++j) {
            sum += g_t[n - j + 1] * binom * g_a[j];
            /* update binom for next j:
               binom = binom * (n - (j-1)) / j  */
            binom = binom * (n - (j-1)) / j;
        }
        g_a[n+1] = bi - sum;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > g_max_n) {
        /* recompute everything up to n */
        if (g_a) DELETE_ARRAY(g_a);
        if (g_t) DELETE_ARRAY(g_t);
        compute_t_array(n);
        compute_a_array(n);
        g_max_n = n;
    }
    return g_a[n];
}
