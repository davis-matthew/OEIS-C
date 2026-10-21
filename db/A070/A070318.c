/*
 * A070318  a(n) = Max_{k=1..n} (sigma(k)-k) where sigma(k)-k is the sum of
 *          proper divisors of k.
 * https://oeis.org/A070318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Max_{1 ≤ k ≤ n} (σ(k) - k), where σ(k) is the sum of all
 *            positive divisors of k
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate array for σ(1..n).  Use calloc so all entries start at 0. */
    seq_t *sigma = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!sigma) return 0;  /* Allocation failed – return 0 as a safe fallback. */

    /* Compute σ(k) by adding each divisor d to all its multiples. */
    for (idx_t d = 1; d <= n; ++d)
    {
        for (idx_t m = d; m <= n; m += d)
            sigma[m] += (seq_t)d;
    }

    /* Find the maximum of σ(k)-k for k = 1..n. */
    seq_t best = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t val = sigma[k] - (seq_t)k;
        if (val > best) best = val;
    }

    free(sigma);
    return best;
}
