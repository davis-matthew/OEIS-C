/*
 * A092509  Möbius transform of sequence A008475.
 * https://oeis.org/A092509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n=1 or not a prime power; a(n) = n if n is prime;
 *            a(n) = n*(1-1/p) if n = p^k with k≥2
 * Range:     u8 n <= 255, u16 n <= 9622, u32 n <= 9622, u64 n <= 9622,
 *            u128 n <= 9622, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* Find the smallest prime factor of n. */
    uint64_t spf = _smallest_prime_factor((uint64_t)n);
    if (spf == n)            /* n is prime. */
        return (seq_t)n;

    /* Check if n is a pure power of its smallest prime factor. */
    uint64_t tmp = n;
    while (tmp % spf == 0)
        tmp /= spf;
    if (tmp != 1)            /* n has another prime factor. */
        return (seq_t)0;

    /* n = p^k with k ≥ 2.  Return n*(1-1/p) = n - n/p. */
    seq_t sn  = (seq_t)n;
    seq_t ssp = (seq_t)spf;
    return sn - (sn / ssp);
}
