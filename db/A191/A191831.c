/*
 * A191831  a(n) = Sum_{i+j=n, i,j >= 1} tau(i)*sigma(j), where tau() =
 *          A000005(), sigma() = A000203().
 * https://oeis.org/A191831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i+j=n, i,j >= 1} tau(i)*sigma(j)
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute A191831 value for given n (n >= 1) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t limit = n;

    /* Allocate arrays for tau and sigma */
    seq_t *tau   = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    seq_t *sigma = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!tau || !sigma) return 0;   /* fallback, should not happen */

    for (idx_t i = 0; i <= limit; ++i) {
        tau[i]   = 0;
        sigma[i] = 0;
    }

    /* Sieve to compute divisor count and sum of divisors */
    for (idx_t k = 1; k <= limit; ++k) {
        for (idx_t m = k; m <= limit; m += k) {
            tau[m]   += (seq_t)1;
            sigma[m] += (seq_t)k;
        }
    }

    /* Compute convolution sum */
    seq_t total = 0;
    for (idx_t i = 1; i < n; ++i) {
        idx_t j = n - i;
        total += tau[i] * sigma[j];
    }

    free(tau);
    free(sigma);
    return total;
}
