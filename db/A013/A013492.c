/*
 * A013492  exp(arcsinh(x)-log(x+1)) =
 *          1+1/2!*x^2-3/3!*x^3+9/4!*x^4-45/5!*x^5...
 * https://oeis.org/A013492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A013492: a(0)=1, a(1)=0; for n≥2: a(n) = -n*a(n-1) + (n%2==0? t[n/2] : 0);
   t(0)=1, t(m) = t(m-1)*(2*m-1)*(3-2*m)  (m≥1)
   The t-recurrence yields t(m) = (2m)!·binom(1/2,m), the
   integer n!·s_n where s_n are the coefficients of sqrt(1+x²). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* internal dynamic arrays for sequence values and the auxiliary t-values */
static seq_t *seq_val = NULL;   /* a(n) values */
static size_t  seq_cap = 0;
static size_t  seq_len = 0;     /* number of computed a-values (seq_len = last computed index +1) */

static seq_t *t_val = NULL;     /* t(m) values, one per even position */
static size_t  t_cap = 0;
static size_t  t_len = 0;       /* number of computed t-values (t_len = last m computed +1) */

/* ensure seq_val has room for indices up to n (inclusive) */
static void ensure_seq_cap(idx_t n)
{
    if (seq_val == NULL) {
        seq_cap = (n >= 1 ? (size_t)(n+1) : 1);
        seq_val = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    } else if (seq_cap <= (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_val = (seq_t *)realloc(seq_val, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ensure t_val has room for values up to m (inclusive) */
static void ensure_t_cap(idx_t m)
{
    if (t_val == NULL) {
        t_cap = (m >= 1 ? (size_t)(m+1) : 1);
        t_val = (seq_t *)malloc(t_cap * sizeof(seq_t));
        t_len = 0;
    } else if (t_cap <= (size_t)m) {
        size_t new_cap = t_cap ? t_cap * 2 : 1;
        while (new_cap <= (size_t)m) new_cap *= 2;
        t_val = (seq_t *)realloc(t_val, new_cap * sizeof(seq_t));
        t_cap = new_cap;
    }
}

/* compute a(n) and any needed smaller values */
static void compute_up_to(idx_t n)
{
    /* compute auxiliary t-values up to m = n/2 */
    idx_t max_m = n / 2;
    if (t_len <= (size_t)max_m) {
        ensure_t_cap(max_m);
        for (idx_t m = (idx_t)t_len; m <= max_m; ++m) {
            if (m == 0) {
                t_val[0] = 1;
            } else {
                /* careful with signed intermediate for (3-2*m) */
                seq_t f1 = (seq_t)(2 * m - 1);
                seq_t f2 = (seq_t)((int64_t)3 - (int64_t)2 * (int64_t)m);
                t_val[m] = t_val[m - 1] * f1 * f2;
            }
        }
        t_len = (size_t)max_m + 1;
    }

    /* compute a-values up to n */
    ensure_seq_cap(n);
    for (idx_t i = (idx_t)seq_len; i <= n; ++i) {
        if (i == 0) {
            seq_val[0] = 1;
        } else if (i == 1) {
            seq_val[1] = 0;
        } else {
            seq_t rhs = ((i % 2) == 0) ? t_val[i / 2] : 0;
            seq_val[i] = - (seq_t)i * seq_val[i - 1] + rhs;
        }
    }
    seq_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (seq_len <= (size_t)n) compute_up_to(n);
    return seq_val[n];
}
