/*
 * A071222  Smallest k such that gcd(n,k) = gcd(n+1,k+1).
 * https://oeis.org/A071222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (smallest prime not dividing n+1) - 1 or equivalently,
 *            the smallest k such that gcd(n,k)=gcd(n+1,k+1)
 * Range:     u8 n <= 9431, u16 n <= 9431, u32 n <= 9431, u64 n <= 9431,
 *            u128 n <= 9431, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* m = n + 1; we test divisibility of m by successive primes. */
    uint64_t m = n + 1;
    for (uint64_t p = 2; ; ++p)
    {
        if (!is_prime_uint64(p))
            continue;
        if (m % p != 0)   /* p does not divide m -> required prime */
            return (seq_t)(p - 1);
    }
}
