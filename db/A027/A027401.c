/*
 * A027401  Number of labeled servers of dimension 14.
 * https://oeis.org/A027401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp( sum_{i=1}^{14} ((1+x)^i-1)/i )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Pre‑computed integers d[k] = sum_{i=k}^{14} binom(i-1,k-1)  (1 ≤ k ≤ 14) */
static seq_t d[15] = {0};

static void init_constants(void)
{
    if (d[1] != 0)          /* already computed */
        return;

    /* Binomial coefficients up to 14 */
    uint64_t C[15][15] = {0};
    for (int n = 0; n <= 14; ++n) {
        C[n][0] = C[n][n] = 1;
        for (int r = 1; r < n; ++r)
            C[n][r] = C[n-1][r-1] + C[n-1][r];
    }

    for (int k = 1; k <= 14; ++k) {
        seq_t sum = 0;
        for (int i = k; i <= 14; ++i)
            sum += (seq_t)C[i-1][k-1];
        d[k] = sum;
    }
}

/* Compute a(n) for n >= 0.  The function is thread‑unsafe but satisfies the
 * requirement that it work for arbitrary call order. */
seq_t a(idx_t n)
{
    init_constants();

    /* Allocate array a[0..n] */
    seq_t *A = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!A)
        return (seq_t)0;          /* allocation failure */

    A[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        idx_t kmax = m < 14 ? m : 14;
        seq_t prod = 1;                /* product for current k */

        for (idx_t k = 1; k <= kmax; ++k) {
            seq_t term = d[k] * prod * A[m - k];
            sum += term;

            if (k < kmax) {
                /* Update product for next k: multiply by (m - k) */
                prod *= (seq_t)(m - k);
            }
        }
        A[m] = sum;
    }

    seq_t result = A[n];
    free(A);
    return result;
}
