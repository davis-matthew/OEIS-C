/*
 * A013342  Expansion of e.g.f.:
 *          exp(sin(x)-arcsin(x))=1-2/3!*x^3-8/5!*x^5+40/6!*x^6-226/7!*x^7...
 * https://oeis.org/A013342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013342: a(0)=1 and for n≥1
   a(n) = Σ_{k odd ≤ n} ( (-1)^((k-1)/2) - ((k-2)!!)^2 ) *
          a(n-k) * binomial(n-1, k-1)
   where (k-2)!! is the double factorial of the odd numbers up to k-2.
   The implementation computes a(n) incrementally and caches results
   so that repeated calls are efficient. */
#include <stdlib.h>
#include <stdint.h>

static size_t a_cap = 0;          /* allocated size of a[]               */
static seq_t *a_arr = NULL;       /* cached a(n) values                 */
static idx_t a_nmax = 0;          /* maximum n computed so far */

/* public function */
seq_t a(idx_t n)
{
    /* initialise cache for a(0) */
    if (a_cap == 0) {
        a_cap = 1;
        a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
        a_arr[0] = (seq_t)1;
        a_nmax = 0;
    }

    /* enlarge cache if necessary */
    if (n >= a_cap) {
        size_t new_cap = (size_t)(n + 1);
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* if already computed, simply return */
    if (n <= a_nmax) return a_arr[n];

    /* compute missing a-values in increasing order */
    for (idx_t cur = a_nmax + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;         /* bin = C(cur-1, 0) */
        seq_t t   = (seq_t)1;         /* t = ((cur-1)/2)!??  start with t=1 for k=1 */
        int  sign = 1;                /* sign = (-1)^{m} for current odd k */

        for (idx_t idx = 0; idx < cur; ++idx) {
            if ((idx & 1) == 0) {                 /* k = idx+1 is odd */
                idx_t k = idx + 1;
                seq_t s_k = (seq_t)sign - t;      /* s_k = (-1)^m - t_k */
                sum += s_k * a_arr[cur - k] * bin;

                /* update t and sign for next odd k (k+2) */
                seq_t k_sq = (seq_t)k * (seq_t)k;
                t = t * k_sq;
                sign = -sign;
            }
            /* update binomial coefficient for next idx */
            if (idx + 1 < cur) {
                bin = bin * (seq_t)(cur - 1 - idx);
                bin = bin / (seq_t)(idx + 1);
            }
        }
        a_arr[cur] = sum;
    }

    a_nmax = n;
    return a_arr[n];
}
