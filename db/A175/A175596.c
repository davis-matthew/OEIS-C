/*
 * A175596  Partial products of A007425.
 * https://oeis.org/A175596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} ( sum_{d|i} tau(d) ) where * tau(d) is
 *            the number of divisors of d. * This is the product of the
 *            ordered factorisation counts of each * integer up to n (OEIS
 *            A175596)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* offset is 1 */

    /* Allocate temporary arrays for divisor counts and the sum
     * of divisor counts over all divisors of each i.  The extra
     * 1 is for 0‑indexing convenience; index 0 will remain unused. */
    size_t sz = (size_t)n + 1;
    seq_t *tau = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *S   = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!tau || !S) {                   /* allocation error */
        /* In a production file you might abort or return 0. */
        return (seq_t)0;
    }

    /* Compute tau[i] = number of divisors of i for all 1 <= i <= n. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            tau[j] += 1;
        }
    }

    /* Compute S[i] = sum_{d|i} tau[d] for each i. */
    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t m = d; m <= n; m += d) {
            S[m] += tau[d];
        }
    }

    /* Compute the cumulative product a(n) = Π_{i=1..n} S[i] */
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        prod *= S[i];
    }

    free(tau);
    free(S);
    return prod;
}
