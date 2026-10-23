/*
 * A035322  Sum of composite divisors of n that are less than n and are not
 *          primes nor prime powers.
 * https://oeis.org/A035322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of composite divisors d of n with d < n and d is not
 *            a prime power
 * Range:     u8 n <= 167, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_two_distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: for each divisor d < n, check if it has at least two distinct prime factors;
   if so, add to the sum. Complexity is fine for n up to about 10000. */

#include <stdbool.h>
#include "../utils/has_two_distinct_prime_factors.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            /* Process the divisor i */
            if (has_two_distinct_prime_factors(i))
                sum += (seq_t)i;

            /* Process the paired divisor j = n/i, unless it equals i or n */
            idx_t j = n / i;
            if (j != i && j != n && has_two_distinct_prime_factors(j))
                sum += (seq_t)j;
        }
    }
    return sum;
}
