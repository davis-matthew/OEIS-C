/*
 * A231173  G.f.: Sum_{n>=0} x^n * Product_{k=1..n} (k - 2*x) / (1 - 2*k*x).
 * https://oeis.org/A231173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A231173
    G.f.:  Sum_{n>=0} x^n * Prod_{k=1..n} (k-2x)/(1-2k x).
    The following implementation computes a(n) by a direct
    dynamic‑programming evaluation of the product.
    The recurrence for the coefficients C_k[d] of
          Q_k(x) = Prod_{i=1..k} (i-2x)/(1-2i x)
    is:
          C_0[0] = 1
          C_k[0]   = k * C_{k-1}[0]
          C_k[d>0] = k*C_{k-1}[d] + 2*(k^2-1) * Σ_{j=1}^d C_{k-1}[d-j] * (2k)^{j-1}
    The desired term a(n) equals Σ_{k=0..n} C_k[n-k].
    The algorithm runs in O(n^2) time and uses only
    integer arithmetic.      */

#include <stdlib.h>
#include <stdint.h>

/*  seq_t and idx_t are defined by the build system.  */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate three arrays of size n+1.  */
    size_t N = (size_t)n;
    seq_t *prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *ans  = (seq_t *)calloc(N + 1, sizeof(seq_t));

    if (!prev || !cur || !ans) {
        /* If allocation fails, return 0.  */
        return (seq_t)0;
    }

    /* Initial product Q0(x) = 1.  */
    prev[0] = (seq_t)1;
    ans[0] += prev[0];          /* k = 0 contributes only to a0 */

    for (idx_t k = 1; k <= n; ++k) {
        size_t max_d = N - (size_t)k;     /* we need coefficients up to degree n-k */

        /* Reset the running sum S for the recurrence of C_k[d] */
        seq_t S = (seq_t)0;

        /* d = 0 */
        cur[0] = (seq_t)k * prev[0];
        ans[(size_t)k] += cur[0];

        /* Precompute constants used in each iteration */
        seq_t two_k   = (seq_t)2 * (seq_t)k;          /* 2k  */
        seq_t coeff   = (seq_t)2 * (((seq_t)k * (seq_t)k) - (seq_t)1); /* 2*(k^2-1) */

        /* For d >= 1 */
        for (size_t d = 1; d <= max_d; ++d) {
            /* S = (2k)*S + prev[d-1]  (recurrence for the geometric sum) */
            S = two_k * S + prev[d - 1];

            /* C_k[d] = k*prev[d] + coeff * S */
            cur[d] = (seq_t)k * prev[d] + coeff * S;

            /* accumulate contribution to a(n) */
            ans[(size_t)k + d] += cur[d];
        }

        /* Prepare for next iteration:  (prev <-> cur)  */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        /* cur will be overwritten in the next loop iteration */
    }

    /* Result is a(n) = ans[n] */
    seq_t result = ans[N];

    /* Free allocated memory */
    free(prev);
    free(cur);
    free(ans);

    return result;
}
