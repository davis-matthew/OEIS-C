/*
 * A068905  Binomial(sigma(n),omega(n)), where sigma(n) is the sum of divisors
 *          of n (A000203) and omega the number of distinct prime factors
 *          (A001221).
 * https://oeis.org/A068905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(sigma(n), omega(n))
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 209, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial, count_distinct_primes, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    idx_t omega = (idx_t)count_distinct_primes(n);
    return binomial((idx_t)sigma, omega);
}
