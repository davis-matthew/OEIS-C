/*
 * A074592  Smallest prime factors of numbers that are not prime powers.
 * https://oeis.org/A074592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime factor of the nth positive integer * that
 *            is NOT a prime power. (OEIS A074592)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Smallest prime factor of n (n >= 2).  For n == 1 returns 1,
 * though n==1 will never be queried as we start from the first
 * non‑prime‑power (which is 6). */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n % 2ULL == 0ULL)
        return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
    {
        if (n % d == 0ULL)
            return d;
    }
    return n;  /* n is prime */
}

/* Returns true iff v is a prime power (including primes). */
static bool is_prime_power(uint64_t v)
{
    if (v <= 1ULL)
        return true;          /* 1 is considered a prime power here */
    uint64_t p = smallest_prime_factor(v);
    /* Divide out all copies of p */
    while (v % p == 0ULL)
        v /= p;
    return v == 1ULL;
}

/* Public function requested.  Works for any seq_t and idx_t defined by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* outside defined offset */

    uint64_t candidate = 0;
    uint64_t found   = 0;

    while (found < n)
    {
        ++candidate;
        if (!is_prime_power(candidate))
        {
            ++found;
            if (found == n)
                return (seq_t)smallest_prime_factor(candidate);
        }
    }

    /* Should never reach here */
    return (seq_t)0;
}
