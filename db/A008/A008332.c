/*
 * A008332  Sum of divisors of p-1, p prime.
 * https://oeis.org/A008332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of divisors of (prime(n) - 1)
 * Range:     u8 n <= 28, u16 n <= 2116, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* get the nth prime (1-indexed) */
    uint32_t p = get_prime(n);
    /* compute the sum of divisors of p-1 */
    return sigma_of((idx_t)(p - 1));
}
