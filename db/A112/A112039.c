/*
 * A112039  Let b(0)=1/2, b(n) = b(n-1) + Prime[n]/2; a(n)=b(2*n).
 * https://oeis.org/A112039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + ∑_{i=1}^{2n} Prime[i]) / 2
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A112039 – uses a small sieve to sum the first 2n primes.   */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/simple_sieve.h"
#include "../utils/estimate_prime_upper_bound.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not defined for offset 0 */

    size_t m = (size_t)((idx_t)2 * n);         /* number of primes required */

    /* Estimate an upper bound on the m‑th prime. */
    uint64_t upper = estimate_prime_upper_bound(m);
    uint32_t limit = (uint32_t)upper;

    uint32_t *primes = NULL;
    size_t count = 0;

    /* Sieve once with the estimate. */
    simple_sieve(limit, &primes, &count);

    /* If the estimate was too low, double the limit until enough primes are found. */
    while (count < m) {
        limit *= 2;
        free(primes);
        simple_sieve(limit, &primes, &count);
    }

    seq_t sum = 0;
    for (size_t i = 0; i < m; ++i)
        sum += (seq_t)primes[i];

    free(primes);

    return (sum + 1) / 2;
}
