/*
 * A013286  cos(log(x+1)-tanh(x))=1-3/4!*x^4+40/5!*x^5-250/6!*x^6+1008/7!*x^7...
 * https://oeis.org/A013286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A013286
 *   cos(log(x+1)-tanh(x)) = 1 - 3/4!*x^4 + 40/5!*x^5 - 250/6!*x^6 + 1008/7!*x^7 ...
 *   We compute the Eulerian (exponential‑generating) coefficients a(n) such that
 *   a(n) is the integer in front of x^n / n! in the expansion of the above function.
 *
 *   The computation uses only integer arithmetic supported by the seq_t type.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return 1;

    /* Allocate arrays of size n+1 (indices 0..n) */
    seq_t *l = (seq_t *)malloc((n + 1) * sizeof(seq_t)); /* log(1+x) EG coeffs */
    seq_t *t = (seq_t *)malloc((n + 1) * sizeof(seq_t)); /* tanh EG coeffs   */
    seq_t *g = (seq_t *)malloc((n + 1) * sizeof(seq_t)); /* A(x)=log-tanh EG coeffs */
    seq_t *y = (seq_t *)malloc((n + 1) * sizeof(seq_t)); /* cos(A) EG coeffs   */
    seq_t *s = (seq_t *)malloc((n + 1) * sizeof(seq_t)); /* sin(A) EG coeffs   */

    /* ------------------------------------------------------------------ */
    /* 1. EG coefficients of log(1+x):  l_k = (-1)^{k-1} (k-1)! for k>=1 */
    l[0] = 0; /* log(1+x) has no constant term */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t fact = 1;
        for (idx_t i = 1; i < k; ++i) fact *= (seq_t)i;          /* (k-1)!   */
        if ((k & 1) == 0) fact = -fact;                           /* sign change for even k */
        l[k] = fact;
    }

    /* ------------------------------------------------------------------ */
    /* 2. EG coefficients of tanh(x) via recurrence:
     *    t_{n+1} = delta_{n,0} - sum_{k=0}^n C(n,k) t_k t_{n-k}
     */
    t[0] = 0; /* tanh(0)=0 */
    for (idx_t n0 = 0; n0 < n; ++n0) {
        /* Compute binomial coefficients for the current n0 */
        seq_t binom = 1; /* C(n0,0) */
        seq_t sum = 0;
        for (idx_t k = 0; k <= n0; ++k) {
            sum += binom * t[k] * t[n0 - k];
            /* Update binom for k -> k+1 */
            if (k < n0) {
                binom = binom * (seq_t)(n0 - k) / (seq_t)(k + 1);
            }
        }
        /* delta_{n0,0} is 1 only when n0==0 */
        seq_t delta = (n0 == 0) ? 1 : 0;
        t[n0 + 1] = delta - sum;
    }

    /* ------------------------------------------------------------------ */
    /* 3. Compute A(x) = log(1+x) - tanh(x)  (EG coefficients) */
    g[0] = 0; /* A(0)=0 */
    for (idx_t k = 1; k <= n; ++k) {
        g[k] = l[k] - t[k];
    }

    /* ------------------------------------------------------------------ */
    /* 4. Compute y = cos(A) and s = sin(A) using recurrence:
     *    y_{n+1} = - sum_{k=0}^n C(n,k) s_k * g_{n-k+1}
     *    s_{n+1} =   sum_{k=0}^n C(n,k) y_k * g_{n-k+1}
     */
    y[0] = 1; /* cos(0) = 1 */
    s[0] = 0; /* sin(0) = 0 */

    for (idx_t n1 = 0; n1 < n; ++n1) {
        /* Compute binomial coefficients once per n1 */
        seq_t binom = 1; /* C(n1,0) */
        seq_t y_sum = 0;
        seq_t s_sum = 0;
        for (idx_t k = 0; k <= n1; ++k) {
            seq_t g_term = (n1 - k + 1 <= n) ? g[n1 - k + 1] : 0;
            /* For y_{n1+1} */
            y_sum -= binom * s[k] * g_term;
            /* For s_{n1+1} */
            s_sum += binom * y[k] * g_term;
            /* Update binom for next k */
            if (k < n1) {
                binom = binom * (seq_t)(n1 - k) / (seq_t)(k + 1);
            }
        }
        y[n1 + 1] = y_sum;
        s[n1 + 1] = s_sum;
    }

    seq_t result = y[n];

    /* ------------------------------------------------------------------ */
    /* Free memory */
    free(l);
    free(t);
    free(g);
    free(y);
    free(s);

    return result;
}
