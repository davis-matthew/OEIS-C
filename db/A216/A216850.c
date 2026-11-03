/*
 * A216850  Number of distinct infinite sets of primes congruent to a subset
 *          of 1..n mod n.
 * https://oeis.org/A216850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^phi(n) - 1) * 2^omega(n)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 974 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the helper functions provided in the
 * utility library: calc_phi for Euler's totient, count_distinct_primes
 * for the number of distinct prime factors, and pow2 for powers of two.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute Euler's totient φ of an odd 64‑bit or larger number m.
   The algorithm uses trial division up to sqrt(m). */
static seq_t calc_phi(seq_t m)
{
    seq_t result = m;
    seq_t p = 2;

    /* Handle factor 2 separately to skip it later (m is odd, but keep robust) */
    if (m % 2 == 0) {
        while (m % 2 == 0)
            m /= 2;
        result -= result / 2;
    }

    /* Trial division by odd primes */
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }

    /* If m > 1, it is a prime factor larger than sqrt(original m) */
    if (m > 1)
        result -= result / m;

    return result;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* Compute 2^exp for any exponent that may not fit in the bit width of seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)2;
    return res;
}

seq_t a(idx_t n)
{
    /* Euler's totient of n */
    seq_t phi = calc_phi((seq_t)n);

    /* Number of distinct prime factors of n */
    int  omega_int = count_distinct_primes((uint64_t)n);
    seq_t omega = (seq_t)omega_int;

    /* Powers of two */
    seq_t two_to_phi   = pow2((idx_t)phi);
    seq_t two_to_omega = pow2((idx_t)omega);

    /* (2^phi - 1) * 2^omega */
    return (two_to_phi - (seq_t)1) * two_to_omega;
}
