/*
 * A009207  Expansion of exp(sin(x))/cos(x).
 * https://oeis.org/A009207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th term of the series expansion of
 *            exp(sin(x))/cos(x). The function computes the integer a(n) for
 *            any n >= 0 using only integer arithmetic. It maintains static
 *            caches so repeated calls with any order are still efficient
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   sign_neg_mag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sign_neg_mag.h"

#ifndef seq_t
/* seq_t is defined in the build environment.  Do not redefine it. */
#endif

/* The main sequence function */
seq_t a(idx_t n)
{
    /* Static caches for computed values */
    static idx_t max_cached = (idx_t)-1;          /* maximum n computed so far */
    static seq_t *e_num = NULL;                  /* e_num[k] = coefficient of e^sin, multiplied by k! */
    static seq_t *c_num = NULL;                  /* c_num[k] = coefficient of sec, multiplied by k! */
    static seq_t *a_cache = NULL;                /* final a(n) values */

    /* If this is the first call, initialise arrays to size 1 */
    if (max_cached == (idx_t)-1) {
        max_cached = 0;
        e_num = (seq_t*)malloc(sizeof(seq_t));
        c_num = (seq_t*)malloc(sizeof(seq_t));
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        e_num[0] = 1;
        c_num[0] = 1;
        a_cache[0] = 1;
    }

    /* Allocate larger arrays if necessary */
    if (n > max_cached) {
        void *tmp;

        tmp = (seq_t*)realloc(e_num, (n + 1) * sizeof(seq_t));
        e_num = (seq_t*)tmp;
        tmp = (seq_t*)realloc(c_num, (n + 1) * sizeof(seq_t));
        c_num = (seq_t*)tmp;
        tmp = (seq_t*)realloc(a_cache, (n + 1) * sizeof(seq_t));
        a_cache = (seq_t*)tmp;

        /* Compute new values from max_cached+1 up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* ---- compute e_num[i] ---- */
            if (i == 0) {
                e_num[0] = 1;
            } else {
                seq_t sum = 0;
                seq_t binom = 1;            /* C(i-1,0) */
                for (idx_t k = 1; k <= i; ++k) {
                    if (k & 1) {            /* k odd */
                        int half = (k - 1) >> 1;       /* (= (k-1)/2) */
                        seq_t term = binom * e_num[i - k];
                        if (half & 1) term = -term;    /* sign = (-1)^half */
                        sum += term;
                    }
                    if (k < i) {
                        /* binom = C(i-1, k) from C(i-1, k-1) */
                        binom *= (i - k);
                        binom /= k;
                    }
                }
                e_num[i] = sum;
            }

            /* ---- compute c_num[i] ---- */
            if (i == 1) {
                c_num[1] = 0;
            } else if (i >= 2) {
                seq_t sum = 0;
                seq_t binom = 1;            /* C(i,0) */
                for (idx_t k = 1; k <= i; ++k) {
                    /* update binom to C(i,k) */
                    binom *= (i - k + 1);
                    binom /= k;
                    if ((k & 1) == 0) {     /* k even */
                        int exp = (k >> 1) + 1;       /* k/2 + 1 */
                        seq_t term = binom * c_num[i - k];
                        if (exp & 1) term = -term;    /* sign = (-1)^{exp} */
                        sum += term;
                    }
                }
                c_num[i] = sum;
            }

            /* ---- compute a_cache[i] ---- */
            {
                seq_t sum = 0;
                seq_t binom = 1;            /* C(i,0) */
                for (idx_t k = 0; k <= i; ++k) {
                    sum += e_num[k] * c_num[i - k] * binom;
                    if (k < i) {
                        binom *= (i - k);
                        binom /= (k + 1);
                    }
                }
                a_cache[i] = sum;
            }
        }

        max_cached = n;
    }

    return a_cache[n];
}
