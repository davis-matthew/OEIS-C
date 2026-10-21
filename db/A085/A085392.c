/*
 * A085392  a(n) = largest prime divisor of n, or 1 if n is 1 or a prime.
 * https://oeis.org/A085392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n <= 1 or n is prime; otherwise largest prime
 *            divisor of n
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/largest_prime_divisor.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    if (is_prime(n))
        return (seq_t)1;

    uint64_t lp = largest_prime_divisor((uint64_t)n);
    return (seq_t)lp;
}
