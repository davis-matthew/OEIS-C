/*
 * A049175  Revert transform of 2*x*(1 - x - x^3 + x^4)-x/(1+x).
 * https://oeis.org/A049175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   f_sign
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A049175
 *  a(n) is the n-th coefficient in the compositional inverse of
 *  the series g(x) = 2*x*(1 - x - x³ + x⁴) - x/(1+x).
 *  Lagrange inversion gives:
 *      a(n) = [x^{n-1}] (x/g(x))ⁿ / n
 *  The code implements this formula directly.
 */

#include <stdlib.h>
#include "../utils/f_sign.h"

/* series returned: a(idx_t n), external linkage */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, a(0) unused */

    size_t N = (size_t)n;           /* we only need terms up to degree n-1 */

    /* 1. build the series G(x)=g(x)/x = 1 + Σ_{k≥1} f_{k+1} x^k  */
    int   *Gsign = (int *)malloc(N * sizeof(int));  /* sign of G coefficients */
    seq_t *q     = (seq_t *)malloc(N * sizeof(seq_t));/* r(x)=1/G(x) */
    seq_t *r     = (seq_t *)malloc(N * sizeof(seq_t));/* 1/G(x) = r(x)     */
    seq_t *cur   = (seq_t *)malloc(N * sizeof(seq_t));
    seq_t *next  = (seq_t *)malloc(N * sizeof(seq_t));

    if (!Gsign || !q || !r || !cur || !next) {
        /* allocation failure – return 0 */
        return 0;
    }

    Gsign[0] = 1;                 /* G(0) = 1 */
    for (size_t i = 1; i < N; ++i) /* G[i] = f_{i+1} */
        Gsign[i] = f_sign((int)(i + 1));

    /* 2. compute q = 1/G via series inversion */
    q[0] = 1;
    for (size_t k = 1; k < N; ++k) {
        seq_t sum_pos = 0, sum_neg = 0;
        for (size_t i = 1; i <= k; ++i) {
            if (Gsign[i] == -1)
                sum_neg += q[k - i];
            else
                sum_pos += q[k - i];
        }
        /* q[k] = sum_neg - sum_pos  (guaranteed non‑negative) */
        q[k] = sum_neg - sum_pos;
    }

    /* 3. r(x) = 1/G(x)  (same as q) */
    for (size_t i = 0; i < N; ++i)
        r[i] = q[i];

    /* 4. initialise cur = r (power 1) */
    for (size_t i = 0; i < N; ++i)
        cur[i] = r[i];

    /* 5. handle n==1 directly */
    if (n == 1) {
        seq_t result = cur[0] / (seq_t)1;   /* cur[0] == 1 */
        free(Gsign); free(q); free(r); free(cur); free(next);
        return result;
    }

    seq_t result = 0;

    /* 6. iterate powers 2 .. n, keeping only coefficients up
       to degree n-1.  After computing power k we get a_k. */
    for (size_t power = 2; power <= n; ++power) {
        /* convolution cur * r, truncate to degree N-1 */
        for (size_t i = 0; i < N; ++i)
            next[i] = 0;
        for (size_t i = 0; i < N; ++i) {
            seq_t ci = cur[i];
            if (ci == 0) continue;
            size_t lim = N - i;
            for (size_t j = 0; j < lim; ++j)
                next[i + j] += ci * r[j];
        }
        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;

        if (power == n) {
            result = cur[power - 1] / (seq_t)power;
            break;
        }
    }

    free(Gsign); free(q); free(r); free(cur); free(next);
    return result;
}
