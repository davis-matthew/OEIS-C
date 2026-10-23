/*
 * A120007  Mobius transform of sum of prime factors of n with multiplicity
 *          (A001414).
 * https://oeis.org/A120007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p if n is a prime power p^k (k>0); otherwise 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the smallest prime factor of n (or n if n is prime). */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n < 2) return n;
    for (uint64_t i = 2; (unsigned __int128)i * i <= n; ++i)
        if (n % i == 0)
            return i;
    return n;  /* n is prime */
}

seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 0 */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t p  = smallest_prime_factor(nn);

    /* If n itself is prime, it's a prime power with exponent 1. */
    if (p == nn)
        return (seq_t)p;

    /* Remove all occurrences of the prime factor p. */
    uint64_t tmp = nn;
    while (tmp % p == 0)
        tmp /= p;

    /* If the remainder is 1, n is a pure power of p. */
    if (tmp == 1)
        return (seq_t)p;

    /* Mixed prime factors: result is 0. */
    return (seq_t)0;
}
