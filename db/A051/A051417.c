/*
 * A051417  Quotients of consecutive values of lcm {1, 3, 5 ..., 2n-1} or
 *          A025547(n+1)/A025547(n).
 * https://oeis.org/A051417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 1 if 2*n+1 is a product of two or more different primes;
 *            otherwise a(n) = the unique prime factor of 2*n+1 (i.e. if 2*n+1
 *            is a prime power p^k with k≥1, return p)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 2*n + 1 (always odd, ≥3 for n≥1) */
    idx_t m = (idx_t)2 * n + 1;
    idx_t rem = m;
    idx_t first_prime = 0;      /* the first (and only) prime factor seen */
    idx_t p;

    /* trial division using odd trial divisors */
    for (p = 3; p * p <= rem; p += 2)
    {
        if (rem % p == 0)
        {
            first_prime = p;
            /* divide out all powers of p */
            while (rem % p == 0)
                rem /= p;
            /* if a second distinct factor appears, answer is 1 */
            if (rem > 1)
                return (seq_t)1;
            /* fully reduced to a prime power */
            return (seq_t)first_prime;
        }
    }

    /* If we reached here, m has no odd divisor ≤ sqrt(rem).
       Thus m is either prime or 2 (impossible here) or 1 (impossible). */
    /* If rem == m, m was prime. */
    return (seq_t)m;
}
