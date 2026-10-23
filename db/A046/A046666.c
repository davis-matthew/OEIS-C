/*
 * A046666  a(n) = n - (smallest prime dividing n).
 * https://oeis.org/A046666
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - smallest prime dividing n (0 for primes)
 * Range:     u8 n <= 257, u16 n <= 8126, u32 n <= 8126, u64 n <= 8126,
 *            u128 n <= 8126, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Offset starts at 2; values for n < 2 are irrelevant. */
    if (n < 2)
        return 0;

    /* Compute the smallest prime factor of n.
       _smallest_prime_factor returns n when n is prime. */
    uint64_t spf = _smallest_prime_factor((uint64_t)n);

    /* If n is prime, result is 0; otherwise subtract the factor. */
    if (spf >= (uint64_t)n)
        return 0;

    return (seq_t)n - (seq_t)spf;
}
