/*
 * A009624  Expansion of sinh(x)*exp(tan(x)).
 * https://oeis.org/A009624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A009624
   Sequence of coefficients of the power series
   sinh(x) * exp(tan(x)).  The double‐convolution
   for exponential generating functions is used:
   a(n) = Σ_{odd k ≤ n} C(n,k) * e[n-k],
   where e is the EGF of exp(tan(x)) and is obtained
   from its derivation
   e[0] = 1,
   e[m] = Σ_{j=1..m} C(m-1 , j-1) * t[j] * e[m-j],
   and t is the EGF of tan(x) derived from
   tan'(x) = 1 + tan²(x):
   t[0] = 0,
   t[1] = 1,
   t[m+1] = Σ_{k=0..m} C(m , k) * t[k] * t[m-k]   (m ≥ 1).
   All coefficients are computed in type seq_t, using only
   integer arithmetic. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t cache = 0;
    static seq_t *t = NULL;   /* coefficients of tan(x) EGF  */
    static seq_t *e = NULL;   /* coefficients of exp(tan(x)) EGF */
    static seq_t *A = NULL;   /* desired sequence values      */

    /* initialise storage if needed */
    if (t == NULL) {
        t = alloc_seq_array(1);
        e = alloc_seq_array(1);
        A = alloc_seq_array(1);
        t[0] = 0;
        e[0] = 1;
        A[0] = 0;
        cache = 0;
    }
    /* extend cache up to n */
    if (n > cache) {
        /* allocate new larger arrays */
        seq_t *t_new = alloc_seq_array(n + 1);
        seq_t *e_new = alloc_seq_array(n + 1);
        seq_t *A_new = alloc_seq_array(n + 1);
        /* copy already computed data */
        for (idx_t i = 0; i <= cache; ++i) {
            t_new[i] = t[i];
            e_new[i] = e[i];
            A_new[i] = A[i];
        }
        /* compute missing values */
        for (idx_t i = cache + 1; i <= n; ++i) {
            /* -------- tan(x) coefficients t[i] -------- */
            if (i == 1) {
                t_new[1] = 1;
            } else {
                idx_t m = i - 1;              /* m = i-1 in recurrence */
                seq_t sum = 0;
                seq_t bin = 1;                /* binomial C(m,0) */
                for (idx_t k = 0; k <= m; ++k) {
                    if (k > 0) {
                        bin = bin * (seq_t)(m - k + 1) / (seq_t)k;
                    }
                    if (t_new[k] && t_new[m - k]) {
                        sum += bin * t_new[k] * t_new[m - k];
                    }
                }
                t_new[i] = sum;
            }

            /* -------- exp(tan(x)) coefficients e[i] -------- */
            if (i == 0) {
                e_new[0] = 1;
            } else {
                seq_t sum = 0;
                seq_t bin = 1;                /* binomial C(i-1,0) */
                for (idx_t j = 1; j <= i; ++j) {
                    if (j > 1) {
                        bin = bin * (seq_t)(i - 1 - (j - 2)) / (seq_t)(j - 1);
                    }
                    if (t_new[j]) {
                        sum += bin * t_new[j] * e_new[i - j];
                    }
                }
                e_new[i] = sum;
            }

            /* -------- desired sequence A[i] -------- */
            if (i == 0) {
                A_new[0] = 0;
            } else {
                seq_t sum = 0;
                seq_t bin = 1;                /* binomial C(i,0) */
                for (idx_t k = 0; k <= i; ++k) {
                    if (k > 0) {
                        bin = bin * (seq_t)(i - k + 1) / (seq_t)k;
                    }
                    if ((k & 1) && k >= 1 && e_new[i - k]) {
                        sum += bin * e_new[i - k];
                    }
                }
                A_new[i] = sum;
            }
        }
        /* replace old arrays */
        free_seq_array(t);
        free_seq_array(e);
        free_seq_array(A);
        t = t_new;
        e = e_new;
        A = A_new;
        cache = n;
    }
    return A[n];
}
