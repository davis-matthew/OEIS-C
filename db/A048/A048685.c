/*
 * A048685  a(n) is the number of times the maximum value of Omega(binomial(n,
 *          k)) occurs in the n-th row of Pascal's triangle, where Omega(n) is
 *          the number of prime divisors of n counted with multiplicity
 *          (A001222).
 * https://oeis.org/A048685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of times the maximum value of Ω(binomial(n, k))
 *            occurs * in the n-th row of Pascal's triangle
 * Range:     u8 n <= 1189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the recurrence
 *      Ω(C(n, k+1)) = Ω(C(n, k)) + Ω(n – k) – Ω(k + 1)
 * where Ω(m) is the number of prime divisors of m counted with multiplicity.
 * The array omega[i] stores Ω(i) for 1 ≤ i ≤ n. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence is defined for n ≥ 1. */
    if (n == 0) return 0;

    /* Allocate an array for Ω(1)…Ω(n).  Index 0 is unused. */
    size_t sz = (size_t)(n + 1);
    seq_t *omega = (seq_t *)malloc(sz * sizeof(seq_t));
    if (omega == NULL) return 0;

    omega[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        omega[i] = count_prime_factors((seq_t)i);
    }

    /* Trivial case: k = 0.  C(n, 0) = 1, Ω(1) = 0. */
    seq_t maxOmega = 0;
    idx_t  countMax = 1;   /* Ω(C(n,0)) already counted */

    seq_t currentOmega = 0;   /* Ω(C(n,0)) == 0 */

    /* Iterate k = 0 … n−1 to compute C(n, k+1). */
    for (idx_t k = 0; k < n; ++k) {
        seq_t nextOmega = (seq_t)((long long)currentOmega
                                 + (long long)omega[n - k]
                                 - (long long)omega[k + 1]);

        currentOmega = nextOmega;

        if (currentOmega > maxOmega) {
            maxOmega = currentOmega;
            countMax = 1;
        } else if (currentOmega == maxOmega) {
            ++countMax;
        }
    }

    free(omega);
    return (seq_t)countMax;
}
