/*
 * A106215  The first n primes, connected by, from left to right, alternating
 *          + and * signs.
 * https://oeis.org/A106215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p1 + Σ_{k=1}^{⌊(n-1)/2⌋} p_{2k}·p_{2k+1} + (n even ? p_n
 *            : 0); where p_i is the i‑th prime
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/estimate_prime_upper_bound.h"
#include "../utils/simple_sieve.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* guard for offset 1 */

    /* Estimate an upper bound for the n‑th prime. */
    uint64_t ub          = estimate_prime_upper_bound(n);
    uint32_t limit       = (uint32_t)ub;
    uint32_t *primes     = NULL;
    size_t   prime_count = 0;

    /* Generate primes up to the estimate. */
    simple_sieve(limit, &primes, &prime_count);

    /* Ensure we actually have n primes; usually the estimate is enough. */
    while (prime_count < (size_t)n) {
        limit = limit + limit / 2 + 100;   /* increase the bound */
        free(primes);
        simple_sieve(limit, &primes, &prime_count);
    }

    /* Compute the alternating sum/product expression. */
    seq_t sum = (seq_t)primes[0];           /* p1 */
    size_t i  = 1;                           /* index of p2 (0‑based) */

    while (i + 1 < (size_t)n) {
        sum += (seq_t)primes[i] * (seq_t)primes[i + 1];
        i += 2;
    }
    if ((n % 2) == 0) {                     /* add the last prime if n is even */
        sum += (seq_t)primes[n - 1];
    }

    free(primes);
    return sum;
}
