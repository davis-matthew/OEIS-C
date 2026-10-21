/*
 * A058062  Number of distinct prime factors of sigma(n), the sum of the
 *          divisors of n.
 * https://oeis.org/A058062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A000203(n)) -- number of distinct prime factors
 *            of sigma(n)
 * Range:     u8 n <= 188, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   count_distinct_primes, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* sigma_of returns the sum of divisors of n as seq_t. */
    seq_t s = sigma_of(n);

    /* sigma(n) is small enough for the test range; cast to uint64_t for prime enumeration. */
    uint64_t sigma_val = (uint64_t)s;

    /* count_distinct_primes returns the number of distinct prime factors of a uint64_t. */
    int cnt = count_distinct_primes(sigma_val);

    return (seq_t)cnt;
}
