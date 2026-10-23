/*
 * A165713  a(n) = the smallest integer > n that is divisible by exactly the
 *          same number of distinct primes as n is.
 * https://oeis.org/A165713
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer > n with the same number of distinct
 *            prime factors as n
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 2)     /* Undefined for the offset; return n+1 as a safe fallback */
        return (seq_t) (n + 1);

    /* Count distinct prime factors of n */
    int k = count_distinct_primes((uint64_t)n);

    /* Search consecutively for the next integer with the same count */
    uint64_t m = (uint64_t)n + 1;
    while (1) {
        if (count_distinct_primes(m) == k)
            return (seq_t)m;
        ++m;
    }
}
