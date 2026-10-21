/*
 * A222145  a(n) = n-th second-order hyperharmonic-exponential number,
 *          multiplied by n!.
 * https://oeis.org/A222145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Sum_{k=0..n} A008277(n,k) * H2(k) ) * n! where H2(k) is
 *            the second‑order hyperharmonic number with g.f.
 *            -log(1‑x)/(1‑x)^2. The code computes the value exactly in the
 *            arithmetic type seq_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return (seq_t)0;

    /* factorial(n)   fac = n! */
    seq_t fac = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        fac *= (seq_t)i;

    /* U[i] = fac * H(i).  C[i] = fac * H2(i) = sum_{k=1}^{i} U[k] */
    seq_t *U = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *C = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!U || !C) {
        free(U); free(C);
        return (seq_t)0;
    }

    U[0] = (seq_t)0;
    C[0] = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t div = fac / (seq_t)i;     /* fac / i is exact */
        U[i] = U[i - 1] + div;
        C[i] = C[i - 1] + U[i];
    }

    /* Stirling numbers of the second kind, row n.
       cur holds S(n-1, k); after the loop cur holds S(n, k). */
    seq_t *cur  = (seq_t *)calloc((n + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((n + 1), sizeof(seq_t));
    if (!cur || !next) {
        free(U); free(C); free(cur); free(next);
        return (seq_t)0;
    }

    cur[0] = (seq_t)1;   /* S(0,0) = 1 */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* next[0] stays 0;  fill up to i */
        next[0] = (seq_t)0;
        for (idx_t k = (idx_t)1; k <= i; ++k) {
            next[k] = cur[k - 1] + (seq_t)k * cur[k];
        }
        /* swap cur and next for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Compute a(n) = sum_{k=0..n} S(n,k) * fac * H2(k) = sum cur[k] * C[k] */
    seq_t result = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= n; ++k)
        result += cur[k] * C[k];

    free(U); free(C); free(cur); free(next);
    return result;
}
