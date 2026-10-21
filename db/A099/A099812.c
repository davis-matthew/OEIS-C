/*
 * A099812  Number of distinct primes dividing 2n (i.e., omega(2n)).
 * https://oeis.org/A099812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct prime divisors of 2*n (A001221(2*n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    uint64_t val = n * 2ULL;           /* 2*n fits in uint64_t for all tested n */
    int cnt = count_distinct_primes(val);
    return (seq_t)cnt;
}
