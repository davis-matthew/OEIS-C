/*
 * A193529  Sum of odd divisors of sopf(n).
 * https://oeis.org/A193529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd divisors of A008472(n) where * A008472(n) is
 *            the sum of distinct prime factors of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sumDistinctPrimeFactors, sumOddDivisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sumDistinctPrimeFactors.h"
#include "../utils/sumOddDivisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety guard – sequence starts at n=1 */
    seq_t sum_distinct = sumDistinctPrimeFactors(n);
    return sumOddDivisors(sum_distinct);
}
