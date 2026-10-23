/*
 * A035321  Sum of composite divisors of n that are not primes nor prime
 *          powers.
 * https://oeis.org/A035321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of composite divisors of n that are not primes nor
 *            prime powers
 * Range:     u8 n <= 119, u16 n <= 8457, u32 n <= 8457, u64 n <= 8457,
 *            u128 n <= 8457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple primality test for numbers <= 1e12 (fits in 64‑bit) */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0)
            return false;
    return true;
}

/* Return true if n is a prime power (i.e. n = p^k with k >= 1) */
static bool is_prime_power(uint64_t n)
{
    if (n < 2) return false;
    uint64_t rem = n;
    for (uint64_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            while (rem % p == 0) rem /= p;
            return rem == 1;          /* remaining part is 1 => prime power */
        }
    }
    return false;                    /* n is prime, but that case is handled elsewhere */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;   /* cast index to 64‑bit for local use */
    seq_t sum = 0;

    for (uint64_t i = 1; i * i <= N; ++i)
    {
        if (N % i == 0)
        {
            uint64_t d1 = i;
            uint64_t d2 = N / i;

            /* Process d1 */
            if (d1 > 1 && !is_prime(d1) && !is_prime_power(d1))
                sum += (seq_t)d1;

            /* Process d2, avoid double‑counting perfect square */
            if (d2 > 1 && d2 != d1 && !is_prime(d2) && !is_prime_power(d2))
                sum += (seq_t)d2;
        }
    }

    return sum;
}
