/*
 * A131124  Expansion of q^(-1) * (phi(-q) / psi(q^4))^2 in powers of q where
 *          phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A131124
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 76, i32 n <= 268, i64 n <= 968,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) is the coefficient of q^n in the series

        A(q) = 1/q * (phi(-q)/psi(q^4))^2
              = 1/q * Π_{k≥1} (1 + q^k)^(-4) (1 + q^(2k))^(-2)(1 + q^(4k))^(-4)

   The implementation builds the product

        C(x) = Π_{m≥1, c_m>0} (1 - x^m)^{-c_m}

   where 

        c_m = 4   if m ≡ 1,3,5,7 (mod 8)   (odd)
        c_m = 2   if m ≡ 2 (mod 4)
        c_m = 4   if m ≡ 4 (mod 8)
        c_m = 0   if m ≡ 0 (mod 8)

   It then computes the inverse series B(x)=1/C(x);
   B(x)=∑_{n≥0} b_n x^n where b_n = a(n-1).  Thus a(n)=b_{n+1}.
   All arithmetic is performed in the type seq_t, which may be any
   signed integer type or an arbitrary‑precision integer class.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < -1) return (seq_t)0;           /* undefined below offset */
    idx_t need = (idx_t)(n + 2);            /* we need coefficients up to index n+1 */

    /* static cache */
    static seq_t *f = NULL;                 /* coefficients of C(x)   */
    static seq_t *g = NULL;                 /* coefficients of 1/C(x)  */
    static idx_t max_n = 0;                 /* highest index computed */

    if (need > max_n) {
        /* allocate new arrays */
        size_t sz = (size_t)need;
        f = (seq_t *)calloc(sz, sizeof(seq_t));
        g = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!f || !g) { /* allocation failed */
            if (f) free(f);
            if (g) free(g);
            return (seq_t)0;
        }
        max_n = need;

        /* compute C(x) = Π (1 - x^m)^{-c_m}  (c_m > 0) */
        f[0] = (seq_t)1;
        for (idx_t m = 1; m < need; ++m) {
            idx_t cnt;
            if (m % 8 == 0) {
                cnt = 0;
            } else if (m % 2 == 1) {         /* odd */
                cnt = 4;
            } else if (m % 4 == 2) {         /* 2 mod 4 */
                cnt = 2;
            } else {                         /* 4 mod 8 */
                cnt = 4;
            }

            for (idx_t r = 0; r < cnt; ++r) {    /* multiply by (1 - x^m)^-1 cnt times */
                for (idx_t j = m; j < need; ++j) {
                    f[j] += f[j - m];
                }
            }
        }

        /* compute inverse series g   (g[0] = 1,  f * g = 1) */
        g[0] = (seq_t)1;
        for (idx_t k = 1; k < need; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t t = 1; t <= k; ++t) {
                sum += f[t] * g[k - t];
            }
            g[k] = -sum;
        }
    }

    /* a(n) = g[n+1]  (since a(-1)=1 corresponds to g[0]) */
    return g[(idx_t)(n + 1)];
}
