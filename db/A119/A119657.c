/*
 * A119657  Denominator of BernoulliB[10p] divided by 66, where p=Prime[n].
 * https://oeis.org/A119657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Denominator[BernoulliB[10Prime[n]]]/66
 * Range:     u8 n <= 10, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator of a Bernoulli number B_k (k>1 even) is the product of all
   primes q such that q‑1 | k (von Staudt–Clausen theorem).  Thus
   a(n) is the product of all primes q with q‑1 | 10p (p = Prime[n]) except
   for q = 2, 3, and 11, because dividing by 66 removes these three factors. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                /* undefined for n < 1 */

    /* n-th prime (1-indexed). */
    idx_t p = (idx_t)get_prime(n);

    /* Compute N = 10 * p. */
    idx_t N = p * (idx_t)10;

    seq_t prod = (seq_t)1;

    /* Enumerate all positive divisors d of N. */
    for (idx_t d = 1; d <= N / d; ++d) {      /* loop up to sqrt(N) */
        if (N % d != (idx_t)0)
            continue;

        /* First divisor: d */
        idx_t q1 = d + (idx_t)1;
        if (is_prime(q1) && q1 != (idx_t)2 && q1 != (idx_t)3 && q1 != (idx_t)11)
            prod *= (seq_t)q1;

        /* Complementary divisor: N/d */
        idx_t d2 = N / d;
        if (d2 == d)                       /* avoid double counting square root */
            continue;

        idx_t q2 = d2 + (idx_t)1;
        if (is_prime(q2) && q2 != (idx_t)2 && q2 != (idx_t)3 && q2 != (idx_t)11)
            prod *= (seq_t)q2;
    }

    return prod;
}
