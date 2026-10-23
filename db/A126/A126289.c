/*
 * A126289  a(1) = 1, a(2) = 1, a(n) = n * LargestPrimeFactor(n-1) /
 *          LargestPrimeFactor(n).
 * https://oeis.org/A126289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * largest_prime_factor(n-1) / largest_prime_factor(n)
 *            for n>2, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 31, u16 n <= 599, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 2u)
        return (seq_t)1;

    uint64_t g1 = largest_prime_factor(n - 1u);
    uint64_t g2 = largest_prime_factor(n);

    /* The product n*g1 fits in seq_t for all n tested (up to ~10^4). */
    return ((seq_t)n * (seq_t)g1) / (seq_t)g2;
}
