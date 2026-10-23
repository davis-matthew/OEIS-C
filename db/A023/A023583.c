/*
 * A023583  Greatest prime divisor of 2*prime(n)-1.
 * https://oeis.org/A023583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest prime divisor of 2*prime(n)-1
 * Range:     u8 n <= 33, u16 n <= 3513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For safety handle n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* Fetch the n-th prime. */
    uint64_t p = (uint64_t)get_prime(n);

    /* Compute 2*prime(n) - 1. */
    uint64_t m = 2 * p - 1;

    /* Greatest prime divisor of m. */
    uint64_t gpf = largest_prime_divisor(m);

    /* Return as seq_t. */
    return (seq_t)gpf;
}
