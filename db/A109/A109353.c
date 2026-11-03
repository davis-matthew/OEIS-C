/*
 * A109353  a(n) is the sum of the distinct prime divisors of A024619(n).
 * https://oeis.org/A109353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the distinct prime divisors of the nth
 *            non‑prime‑power (A024619(n) is the sequence of positive integers
 *            that are not prime powers)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Determine whether an integer is a prime power.  It returns true for primes
   and for numbers of the form p^k (k ≥ 2), false otherwise. */
static bool is_prime_power(uint64_t x)
{
    if (x < 2) return false;          /* 0 and 1 are not prime powers by definition */
    /* Check divisors up to sqrt(x).  The first divisor found is guaranteed to be
       prime, so we may safely use it to test the power. */
    for (uint64_t d = 2; d * d <= x; ++d)
    {
        if (x % d == 0)
        {
            /* d is the smallest prime factor.  Divide out all copies. */
            uint64_t y = x;
            while (y % d == 0) y /= d;
            return (y == 1);           /* true only if no other distinct factor remained */
        }
    }
    /* No divisor found – x is prime. */
    return true;
}

/* Return the sum of the distinct prime divisors of a(n).  */
seq_t a(idx_t n)
{
    /* Find the nth non‑prime‑power. */
    idx_t count = 0;          /* how many non‑prime‑powers encountered */
    uint64_t candidate = 2;   /* numbers start from 2 */
    uint64_t target = 0;      /* the nth non‑prime‑power found */

    while (count < n)
    {
        if (!is_prime_power(candidate))
        {
            ++count;
            if (count == n) { target = candidate; break; }
        }
        ++candidate;
    }

    /* Factorize target and sum distinct primes. */
    seq_t sum = 0;
    uint64_t tmp = target;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            sum += (seq_t)p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) sum += (seq_t)tmp;   /* remaining prime factor */

    return sum;
}
