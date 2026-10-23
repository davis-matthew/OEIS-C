/*
 * A198952  G.f.: Sum_{n>=0} n! * 3^(n*(n-1)/2) * x^n / Product_{k=1..n} (1 +
 *          k*3^k*x).
 * https://oeis.org/A198952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Sum_{k>=0} k! * 3^{k(k-1)/2} * x^k
 *            / Product_{j=1..k} (1 + j*3^j*x)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate temporary storage for coefficients */
    seq_t *a   = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *cur = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *new_cur = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!a || !cur || !new_cur) {
        free(a); free(cur); free(new_cur);
        return (seq_t)0;            /* graceful failure on OOM */
    }

    /* coefficients of 1/D_{0} simply 1 */
    cur[0] = (seq_t)1;

    /* a(0) = 1 */
    a[0] = (seq_t)1;

    /* running values for factorial and 3-powers   */
    seq_t fact = (seq_t)1;         /* 0! = 1                */
    seq_t pow3_half = (seq_t)1;    /* 3^{k(k-1)/2} incrementally   */
    seq_t pow3_prev = (seq_t)1;    /* 3^{k-1}                 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update factorial */
        fact = fact * k;                     /* k!        */

        /* Update 3^{k(k-1)/2}      */
        pow3_half = pow3_half * pow3_prev;    /* multiply by 3^{k-1} */

        /* 3^k for the next factor */
        seq_t pow3_k = pow3_prev * (seq_t)3; /* 3^k        */
        pow3_prev = pow3_k;                  /* prepare for next iteration */

        /* d = k * 3^k */
        seq_t d = k * pow3_k;

        /* Compute coefficients of 1/D_k up to degree n-k  */
        idx_t max_m = n - k;
        new_cur[0] = (seq_t)1;
        for (idx_t m = 1; m <= max_m; ++m) {
            seq_t old = (m <= n - (k - 1)) ? cur[m] : (seq_t)0;
            new_cur[m] = old - d * new_cur[m - 1];
        }

        /* Contribution of the k-th term to the final sequence */
        seq_t b_k = fact * pow3_half;  /* numerator of the k‑th term */
        for (idx_t m = 0; m <= max_m; ++m)
            a[k + m] += b_k * new_cur[m];

        /* Prepare for next iteration: cur = new_cur  */
        for (idx_t m = 0; m <= max_m; ++m)
            cur[m] = new_cur[m];
    }

    seq_t result = a[n];
    free(a); free(cur); free(new_cur);
    return result;
}
