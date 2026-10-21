/*
 * A086207  Sum of rank(M) over all n X n matrices over GF(4).
 * https://oeis.org/A086207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=1..n} r * [Product_{i=0}^{r-1} (4^n - 4^i)^2] /
 *            [Product_{i=0}^{r-1} (4^r - 4^i)]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (q=4) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Precompute powers of 4 up to n: pow_q[i] = 4^i */
    seq_t *pow_q = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!pow_q) return 0;          /* Allocation failure: return 0 */
    pow_q[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow_q[i] = pow_q[i - 1] * (seq_t)4;

    seq_t binom_q = (seq_t)1;        /* C(n,0) = 1 */
    seq_t prod_gl = (seq_t)1;        /* product_{i=1}^r (4^i - 1) */
    seq_t qpow_gl = (seq_t)1;        /* 4^{r(r-1)/2} */
    seq_t sum     = (seq_t)0;

    for (idx_t r = 1; r <= n; ++r)
    {
        /* Update Gaussian binomial coefficient C(n,r) */
        seq_t numerator   = pow_q[n - (r - 1)] - (seq_t)1; /* 4^{n-r+1} - 1 */
        seq_t denominator = pow_q[r] - (seq_t)1;           /* 4^r - 1 */
        binom_q = (binom_q * numerator) / denominator;

        /* Update GL(r,4) components */
        prod_gl = prod_gl * (pow_q[r] - (seq_t)1);   /* multiply by (4^r - 1) */
        qpow_gl = qpow_gl * pow_q[r - 1];            /* multiply by 4^{r-1} */

        /* Compute N_r = binom_q^2 * prod_gl * qpow_gl */
        seq_t N = binom_q * binom_q;
        N = N * prod_gl;
        N = N * qpow_gl;

        /* Add r * N to sum */
        sum += ((seq_t)r) * N;
    }

    free(pow_q);
    return sum;
}
