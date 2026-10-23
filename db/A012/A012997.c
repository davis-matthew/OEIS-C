/*
 * A012997  Expansion of e.g.f.:
 *          exp(arctan(x)+tan(x))=1+2*x+4/2!*x^2+8/3!*x^3+16/4!*x^4+72/5!*x^5...
 * https://oeis.org/A012997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the n‑th coefficient in the e.g.f. expansion of
 *            exp(arctan(x)+tan(x)) = 1+2x+4/2!x²+8/3!x³+… The coefficients
 *            satisfy a(0) = 1 a(n) = Σ_{k=1}^{n} binom(n‑1, k‑1) * f(k) *
 *            a(n‑k) (n≥1) where f(k) = t_k + (k odd ? (k‑1)! * (–1)^((k‑1)/2)
 *            : 0) The tangent numbers t_k are defined by t_0 = 0,...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>           /* malloc, realloc, exit */

/* Static storage for factorials, tangent numbers and sequence a(n). */
static seq_t *fact_arr = NULL;
static size_t fact_cap = 0;

static seq_t *tan_arr = NULL;
static size_t tan_cap = 0;

static seq_t *a_arr   = NULL;
static size_t a_cap   = 0;

/* Ensure factorials up to n are stored in fact_arr. */
static void ensure_fact(idx_t n)
{
    if (fact_cap > n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(fact_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    for (idx_t i = fact_cap; i <= n; ++i) {
        if (i == 0)
            tmp[i] = 1;
        else
            tmp[i] = tmp[i-1] * (seq_t)i;
    }
    fact_arr = tmp;
    fact_cap = new_cap;
}

/* Ensure tangent numbers up to n are stored in tan_arr. Uses recurrence
   t_{m+1} = Σ_{j=1}^{m‑1} binom(m, j) * t_j * t_{m‑j}. */
static void ensure_tan(idx_t n)
{
    if (tan_cap > n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(tan_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    if (tan_cap == 0) {
        tmp[0] = 0;
        /* start = 0 */
    }
    for (idx_t i = tan_cap; i <= n; ++i) {
        if (i == 0)
            tmp[0] = 0;
        else if (i == 1)
            tmp[1] = 1;
        else {
            seq_t sum = 0;
            idx_t m = i - 1;                     /* we need binom(m, j) */
            if (m >= 2) {
                seq_t binom = (seq_t)m;          /* binom(m,1) */
                for (idx_t j = 1; j <= m - 1; ++j) {
                    /* use j as k in the formula */
                    seq_t prod = binom * tmp[j] * tmp[m - j];
                    sum += prod;
                    if (j < m - 1)
                        binom = binom * (seq_t)(m - j) / (seq_t)(j + 1);
                }
            }
            tmp[i] = sum;
        }
    }
    tan_arr = tmp;
    tan_cap = new_cap;
}

/* Ensure sequence a[0..n] is computed in a_arr. */
static void ensure_a(idx_t n)
{
    if (a_cap > n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    if (a_cap == 0) {
        tmp[0] = 1;      /* a(0) = 1 */
    }
    for (idx_t i = a_cap; i <= n; ++i) {
        if (i == 0)
            tmp[0] = 1;
        else {
            seq_t sum = 0;
            seq_t binom = 1;          /* binom(i‑1,0) */
            for (idx_t k = 1; k <= i; ++k) {
                /* arctan part */
                seq_t arcf = 0;
                if (k & 1) {          /* k is odd */
                    seq_t sign = ((((k - 1) >> 1) & 1) ? -1 : 1);
                    arcf = fact_arr[k - 1] * sign;
                }
                seq_t f_k = tan_arr[k] + arcf;
                sum += f_k * binom * tmp[i - k];
                if (k < i)                        /* next binomial */
                    binom = binom * (seq_t)(i - k) / (seq_t)k;
            }
            tmp[i] = sum;
        }
    }
    a_arr = tmp;
    a_cap = new_cap;
}

/* Main function requested by the platform. */
seq_t a(idx_t n)
{
    ensure_fact(n);
    ensure_tan(n);
    ensure_a(n);
    return a_arr[n];
}
