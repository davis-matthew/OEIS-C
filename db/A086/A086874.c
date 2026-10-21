/*
 * A086874  Seventh power of odd primes.
 * https://oeis.org/A086874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n+1))^7 where prime(1)=2, i.e. the seventh power
 *            of the n-th odd prime
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* fetch the (n+1)-th prime (1 → 2, 2 → 3, …) */
    uint32_t p = get_prime(n + 1);
    /* return its seventh power */
    return pow_uint(p, 7);
}
