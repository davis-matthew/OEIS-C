/*
 * A156554  The number of integer sequences of length d = 2n+1 such that the
 *          sum of the terms is 0 and the sum of the absolute values of the
 *          terms is d-1.
 * https://oeis.org/A156554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = Σ_{k=0..n} C(2*n,n-k)^2 * C(2*n+k-1,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* required by the harness */

static seq_t my_binom(idx_t n, idx_t k) {
    /* multiplicative binomial coefficient: C(n,k) */
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0) return 1;

    idx_t two_n = 2 * n;
    seq_t C1 = my_binom(two_n, n);   /* C(2n, n)   */
    seq_t t  = 1;                    /* C(2n+0-1,0) = 1 */

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* term = C(2n, n-k)^2 * C(2n+k-1,k) */
        seq_t term = C1 * C1;        /* square the first binomial */
        term *= t;                  /* multiply by the second binomial */
        sum += term;

        if (k == n) break;          /* last iteration reached */

        /* update C1 for next k: C(2n, n-(k+1)) = C(2n, n-k) * (n-k) / (n+k+1) */
        seq_t num1 = (seq_t)(n - k);
        seq_t den1 = (seq_t)(n + k + 1);
        C1 = (C1 * num1) / den1;

        /* update t for next k: C(2n+k, k+1) = C(2n+k-1, k) * (2n+k) / (k+1) */
        seq_t num2 = (seq_t)(2 * n + k);
        seq_t den2 = (seq_t)(k + 1);
        t = (t * num2) / den2;
    }

    return sum;
}
