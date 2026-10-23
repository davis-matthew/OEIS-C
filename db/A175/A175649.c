/*
 * A175649  a(n) = sopf(n) + sopf(n+1).
 * https://oeis.org/A175649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sopf(n) + sopf(n+1) where sopf(m) = sum of distinct prime
 *            factors of m
 * Range:     u8 n <= 177, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   distinct_prime_factor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/distinct_prime_factor_sum.h"

seq_t a(idx_t n)
{
    /* n and n+1 fit into uint64_t for all intended test ranges */
    seq_t sum1 = distinct_prime_factor_sum((uint64_t)n);
    seq_t sum2 = distinct_prime_factor_sum((uint64_t)(n + 1));
    return sum1 + sum2;
}
