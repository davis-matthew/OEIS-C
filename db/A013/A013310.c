/*
 * A013310  sin(exp(x)-cos(x))=x+2/2!*x^2-12/4!*x^4-68/5!*x^5-208/6!*x^6...
 * https://oeis.org/A013310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute a(n) = n! * [x^n] sin(exp(x)-cos(x))
   using the recurrence derived from:
      g(x)=sin(f(x)), w(x)=cos(f(x)), f(x)=exp(x)-cos(x)
      g'(x)=w(x)*f'(x),   w'(x) = -g(x)*f'(x)
   with g[0]=0, w[0]=1.
   For n≥0:
      g_{n+1} = Σ_{i=0}^{n} binom(n,i) * w_i * f_{n-i+1}
      w_{n+1} = - Σ_{i=0}^{n} binom(n,i) * g_i * f_{n-i+1}
   where f_k = coefficient of x^k in f(x) * k!:
        f_0 = 0
        f_k = 1  if k odd
        f_k = 2  if k ≡ 2 (mod 4)
        f_k = 0  if k ≡ 0 (mod 4)
   Memoisation is used so that repeated calls are efficient.
*/

#include <stdint.h>
#include <stdlib.h>

/* Forward declarations – seq_t and idx_t are defined by the build system */
static seq_t *g = NULL;
static seq_t *w = NULL;
static seq_t *f = NULL;
static idx_t cap = 0;        /* current allocated size */
static idx_t computed = 0;   /* highest n for which g[n] is known */

/* Allocate room for an array of seq_t of given size */
static seq_t *alloc_seq_array(size_t n)
{
    return (seq_t *)malloc(n * sizeof(seq_t));
}

/* Compute the value of f_k according to the definition above */
static inline seq_t f_value(idx_t k)
{
    if (k == 0) return (seq_t)0;
    if (k & 1) return (seq_t)1;          /* odd */
    if ((k & 3) == 2) return (seq_t)2;   /* k % 4 == 2 */
    return (seq_t)0;                      /* k % 4 == 0 */
}

/* Ensure that arrays are allocated and f-values up to n exist */
static void ensure_capacity(idx_t n)
{
    if (cap <= n) {
        idx_t new_cap = n + 1;
        g = (seq_t *)realloc(g, new_cap * sizeof(seq_t));
        w = (seq_t *)realloc(w, new_cap * sizeof(seq_t));
        f = (seq_t *)realloc(f, new_cap * sizeof(seq_t));

        /* Initialise new entries of f */
        for (idx_t i = cap; i <= n; ++i)
            f[i] = f_value(i);

        /* Initialise g[0] and w[0] once when the first allocation occurs */
        if (cap == 0) {
            g[0] = (seq_t)0;
            w[0] = (seq_t)1;
        }

        cap = new_cap;
    }
}

/* Main function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    ensure_capacity(n);

    /* Compute terms up to n, using the recurrence above */
    while (computed < n) {
        idx_t k = computed;                /* we will compute g[k+1] */
        seq_t sum_g = (seq_t)0;
        seq_t sum_w = (seq_t)0;
        seq_t binom = (seq_t)1;            /* binom(k,0) */

        for (idx_t i = 0; i <= k; ++i) {
            idx_t f_idx = k - i + 1;       /* index for f_{k-i+1} */
            seq_t f_val = f[f_idx];
            seq_t w_val = w[i];
            seq_t g_val = g[i];

            sum_g += binom * w_val * f_val;
            sum_w += binom * g_val * f_val;

            /* update binom(k,i+1) */
            if (i != k) {
                seq_t num = binom * (seq_t)(k - i);
                binom = num / (seq_t)(i + 1);    /* exact division */
            }
        }
        g[k + 1] = sum_g;
        w[k + 1] = -sum_w;
        ++computed;
    }

    return g[n];
}
