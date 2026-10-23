/*
 * A100743  Number of labeled n-vertex graphs without vertices of degree <=1.
 * https://oeis.org/A100743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{s=0}^{n} (-1)^s · C(n,s) · 2^{(n-s)(n-s-1)/2} ·
 *            h_s(n-s+1) where h_0(t)=1, h_1(t)=t, and
 *            h_k(t)=t·h_{k-1}(t)+(k-1)·h_{k-2}(t) This counts labeled graphs
 *            on n vertices with no vertex of degree 0 or 1. The code uses
 *            inclusion‑exclusion on the set of vertices with degree ≤1 and a
 *            simple re...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* precompute 2^{r(r-1)/2} for r = 0..n */
    seq_t *pow2 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    pow2[0] = (seq_t)1;
    for (idx_t r = 1; r <= n; ++r) {
        seq_t factor = ((seq_t)1) << (r - 1);   /* 2^{r-1} */
        pow2[r] = pow2[r - 1] * factor;
    }

    /* precompute binomial coefficients C(n,s) for s = 0..n */
    seq_t *binom = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    binom[0] = (seq_t)1;
    for (idx_t s = 1; s <= n; ++s) {
        binom[s] = binom[s - 1] * (seq_t)(n - s + 1) / (seq_t)s;
    }

    seq_t sum_even = (seq_t)0;
    seq_t sum_odd  = (seq_t)0;

    for (idx_t s = 0; s <= n; ++s) {
        /* compute h_s(t) with t = n-s+1 */
        idx_t t = n - s + 1;
        seq_t h_val;

        if (s == 0) {
            h_val = (seq_t)1;
        } else if (s == 1) {
            h_val = (seq_t)t;
        } else {
            seq_t h_prev2 = (seq_t)1;         /* h_0(t) */
            seq_t h_prev1 = (seq_t)t;          /* h_1(t) */
            for (idx_t k = 2; k <= s; ++k) {
                seq_t cur = h_prev1 * (seq_t)t + h_prev2 * (seq_t)(k - 1);
                h_prev2 = h_prev1;
                h_prev1 = cur;
            }
            h_val = h_prev1;
        }

        seq_t term = binom[s] * pow2[n - s];
        term = term * h_val;

        if ((s & 1) == 0)
            sum_even = sum_even + term;
        else
            sum_odd = sum_odd + term;
    }

    seq_t result = sum_even - sum_odd;

    free(pow2);
    free(binom);
    return result;
}
