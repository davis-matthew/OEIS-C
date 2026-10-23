/*
 * A161657  a(n) = the smallest multiple of {the sum of the distinct prime
 *          divisors of n} that is >= n.
 * https://oeis.org/A161657
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n / s) * s, where s = sum of distinct prime divisors
 *            of n
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_of_distinct_prime_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_of_distinct_prime_divisors.h"

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; any value is acceptable. */
    if (n < 2) return (seq_t)n;

    seq_t s = sum_of_distinct_prime_divisors(n);
    seq_t nseq = (seq_t)n;

    /* Compute ceil(n / s) as (n + s - 1) / s */
    seq_t k = (nseq + s - 1) / s;
    return k * s;
}
