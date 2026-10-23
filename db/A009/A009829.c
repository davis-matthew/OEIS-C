/*
 * A009829  Expansion of e.g.f.: tanh(x)*exp(sinh(x)).
 * https://oeis.org/A009829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n/n! in tanh(x)*exp(sinh(x)) (OEIS
 *            A009829)
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 15, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static size_t cache_size = 0;   /* current allocated size */
    static seq_t *t_arr = NULL;     /* coefficients of tanh(x) EGF */
    static seq_t *f_arr = NULL;     /* coefficients of exp(sinh(x)) EGF */
    static seq_t *a_arr = NULL;     /* desired sequence coefficients */

    /* grow arrays if required */
    if ((idx_t)n >= (idx_t)cache_size) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1;

        /* allocate new storage */
        seq_t *t_new = (seq_t*)malloc(new_size * sizeof(seq_t));
        seq_t *f_new = (seq_t*)malloc(new_size * sizeof(seq_t));
        seq_t *a_new = (seq_t*)malloc(new_size * sizeof(seq_t));

        /* copy old contents */
        if (old_size > 0) {
            memcpy(t_new, t_arr, old_size * sizeof(seq_t));
            memcpy(f_new, f_arr, old_size * sizeof(seq_t));
            memcpy(a_new, a_arr, old_size * sizeof(seq_t));
        }

        /* free old storage */
        free(t_arr);
        free(f_arr);
        free(a_arr);

        t_arr = t_new;
        f_arr = f_new;
        a_arr = a_new;
        cache_size = new_size;

        /* compute new terms */
        for (size_t idx = old_size; idx <= new_size - 1; ++idx) {
            /* ==================
               Compute t(idx)
               ================== */
            if (idx == 0) {
                t_arr[0] = (seq_t)0;          /* tanh(0) = 0   */
            } else if (idx == 1) {
                t_arr[1] = (seq_t)1;          /* derivative term gives 1 */
            } else {
                seq_t sum_t = 0;
                size_t m = idx - 1;               /* n in the recurrence */
                seq_t binom = 1;                  /* binom(n,0) */
                for (size_t k = 0; k <= m; ++k) {
                    sum_t += binom * t_arr[k] * t_arr[m - k];
                    if (k != m)
                        binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
                }
                t_arr[idx] = -sum_t;
            }

            /* ==================
               Compute f(idx)
               ================== */
            if (idx == 0) {
                f_arr[0] = (seq_t)1;          /* exp(sinh(0)) = 1 */
            } else {
                seq_t sum_f = 0;
                size_t m = idx - 1;               /* n in the recurrence */
                seq_t binom = 1;                  /* binom(n,0) */
                for (size_t k = 0; k <= m; ++k) {
                    if ((k & 1) == 0) {
                        sum_f += binom * f_arr[m - k];
                    }
                    if (k != m)
                        binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
                }
                f_arr[idx] = sum_f;
            }

            /* ==================
               Compute a(idx)
               ================== */
            if (idx == 0) {
                a_arr[0] = (seq_t)0;          /* a(0) = 0 */
            } else {
                seq_t sum_a = 0;
                size_t m = idx;                 /* n in the convolution */
                seq_t binom = 1;                /* binom(n,0) */
                for (size_t k = 0; k <= m; ++k) {
                    sum_a += binom * t_arr[k] * f_arr[idx - k];
                    if (k != m)
                        binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
                }
                a_arr[idx] = sum_a;
            }
        }
    }

    return a_arr[(size_t)n];
}
