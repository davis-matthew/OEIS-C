/*
 * A093694  Number of one-element transitions from the partitions of n to the
 *          partitions of n+1 for labeled parts.
 * https://oeis.org/A093694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + sum_{k=1}^n d(k) * p(n−k) where p(i) = number of
 *            partitions of i d(k) = number of divisors of k
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 99, u64 n <= 354,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the A093694 sequence. */
seq_t a(idx_t n)
{
    /* Allocate array for partition numbers p[0..n] */
    seq_t *p = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!p) return 0;                 /* unlikely allocation failure */

    /* p[0] = 1 (empty partition) */
    p[0] = (seq_t)1;

    /* Compute all partitions p[i] using Euler's pentagonal recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;                 /* positive and negative parts of the sum */
        for (idx_t k = 1; ; ++k) {
            /* first generalized pentagonal number */
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = p[i - pent1];
            if (k & 1) pos += term; else neg += term;

            /* second generalized pentagonal number */
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                term = p[i - pent2];
                if (k & 1) pos += term; else neg += term;
            }
        }
        p[i] = pos - neg;                     /* positive >= negative by theory */
    }

    /* Compute divisor counts d[1..n] by a simple sieve */
    unsigned int *d = (unsigned int *)calloc((size_t)(n + 1), sizeof(unsigned int));
    if (!d) { free(p); return 0; }           /* unlikely allocation failure */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) d[j]++;
    }

    /* Compute the total number of parts over all partitions of n:
       T(n) = sum_{k=1}^n d(k) * p(n-k) */
    seq_t T = 0;
    for (idx_t k = 1; k <= n; ++k) {
        T += (seq_t)d[k] * p[n - k];
    }

    /* Result a(n) = p(n) + T(n) */
    seq_t result = p[n] + T;

    /* Free allocated memory */
    free(p);
    free(d);

    return result;
}
