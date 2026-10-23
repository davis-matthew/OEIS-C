/*
 * A012593  cosh(arcsinh(x)*exp(x))=1+1/2!*x^2+6/3!*x^3+21/4!*x^4+60/5!*x^5...
 * https://oeis.org/A012593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] cosh(arcsinh(x)*exp(x)) (sequence A012593)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;     /* a(0) = 1 */

    idx_t N = n;
    idx_t mmax = (N - 1) / 2;

    /* allocate working arrays */
    seq_t *d      = (seq_t *)malloc((mmax + 1) * sizeof(seq_t));  /* d[m] = (-1)^m * C(2m,m)*(2m)! / 4^m */
    seq_t *hprime = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* h'_k = k! * [x^k] (arcsinh(x)*exp(x)) */
    seq_t *s      = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* s[k] = k! * t_k  */
    seq_t *r      = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* r[k] = k! * u_k  */

    /* -------------------------------------------------------------
       1.  Compute d[m]   (recurrence from ( (2m+2)(2m+1) )^2 / (4*(m+1)^2) )
    ------------------------------------------------------------- */
    d[0] = (seq_t)1;
    for (idx_t m = 0; m < mmax; ++m) {
        seq_t factor = (seq_t)(2 * m + 2) * (seq_t)(2 * m + 1);   /* (2m+2)(2m+1) */
        seq_t num    = d[m] * factor * factor;                   /* d[m] * factor^2   */
        seq_t denom  = (seq_t)(m + 1) * (seq_t)(m + 1) * (seq_t)4;
        d[m + 1]     = -num / denom;                            /* exact division   */
    }

    /* -------------------------------------------------------------
       2.  Compute h'_k = sum_{m} d[m] * C(k, 2m+1)   (k from 1..N)
    ------------------------------------------------------------- */
    hprime[0] = (seq_t)0;
    for (idx_t k = 1; k <= N; ++k) {
        seq_t binom = (seq_t)1;      /* C(k,0) */
        seq_t sum   = (seq_t)0;
        for (idx_t r = 1; r <= k; ++r) {
            binom = binom * (seq_t)(k - r + 1) / (seq_t)r;   /* C(k,r) */
            if (r & 1) {                                   /* r = 2m+1 */
                idx_t m = (r - 1) / 2;
                sum += d[m] * binom;
            }
        }
        hprime[k] = sum;
    }

    /* -------------------------------------------------------------
       3.  Compute s[n]  (s[0] = 1)
           Recurrence: s[n] = Σ_{k=1..n} h'_k * s[n-k] * C(n-1, k-1)
    ------------------------------------------------------------- */
    s[0] = (seq_t)1;
    for (idx_t ncur = 1; ncur <= N; ++ncur) {
        seq_t total = (seq_t)0;
        seq_t binom = (seq_t)1;      /* C(ncur-1,0) */
        for (idx_t k = 1; k <= ncur; ++k) {
            total += hprime[k] * s[ncur - k] * binom;
            if (k < ncur) {
                binom = binom * (seq_t)(ncur - k) / (seq_t)k;   /* Next binomial C(ncur-1,k) */
            }
        }
        s[ncur] = total;
    }

    /* -------------------------------------------------------------
       4.  Compute r[n]  (r[0] = 1)
           Recurrence: r[n] = - Σ_{k=1..n} s[k] * r[n-k] * C(n, k)
    ------------------------------------------------------------- */
    r[0] = (seq_t)1;
    for (idx_t ncur = 1; ncur <= N; ++ncur) {
        seq_t total = (seq_t)0;
        seq_t binom = (seq_t)1;      /* C(ncur,0) */
        for (idx_t k = 1; k <= ncur; ++k) {
            binom = binom * (seq_t)(ncur - k + 1) / (seq_t)k;   /* C(ncur,k) */
            total += s[k] * r[ncur - k] * binom;
        }
        r[ncur] = -total;
    }

    /* -------------------------------------------------------------
       5.  Final value: a(n) = (s[n] + r[n]) / 2
    ------------------------------------------------------------- */
    seq_t result = (s[N] + r[N]) / (seq_t)2;

    /* free temporary storage */
    free(d);
    free(hprime);
    free(s);
    free(r);

    return result;
}
