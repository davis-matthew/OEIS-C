/*
 * A078311  Smallest prime factor of n*rad(n)+1, where rad = A007947
 *          (squarefree kernel).
 * https://oeis.org/A078311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime factor of (n * rad(n) + 1) where rad(n) is
 *            the product of the distinct prime factors of n
 * Range:     u8 n <= 21, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                                   */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* static data for a tiny prime sieve up to 10 000 */
static const uint32_t PRIME_LIMIT = 10000;
static bool primes_inited = false;
static uint32_t primes_arr[2000];
static size_t primes_cnt = 0;

/* Initialise the list of primes with a simple sieve.  */
static void
init_primes (void)
{
    if (primes_inited)
        return;
    bool marked[PRIME_LIMIT + 1];
    memset (marked, 0, sizeof (marked));
    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!marked[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                marked[j] = true;
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!marked[i])
            primes_arr[cnt++] = i;
    primes_cnt = cnt;
    primes_inited = true;
}

/* Compute rad(n) as the product of distinct prime factors.  */
static seq_t
rad_uint64 (uint64_t n)
{
    init_primes ();
    seq_t r = 1;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes_arr[i] * primes_arr[i] <= n; ++i)
    {
        uint64_t p = primes_arr[i];
        if (n % p == 0)
        {
            r *= (seq_t)p;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)          /* remaining prime factor > sqrt(original n) */
        r *= (seq_t)n;
    return r;
}

/* Smallest prime factor of a 64‑bit value via trial division.  */
static seq_t
smallest_prime_factor_uint64 (uint64_t v)
{
    if (v <= 1)
        return 1;
    if ((v & 1) == 0)
        return 2;
    init_primes ();
    for (size_t i = 1; i < primes_cnt && (uint64_t)primes_arr[i] * primes_arr[i] <= v; ++i)
    {
        uint64_t p = primes_arr[i];
        if (v % p == 0)
            return (seq_t)p;
    }
    /* v itself is prime */
    return (seq_t)v;
}

/* Entry point for the sequence.  */
seq_t
a (idx_t n)
{
    /* n ≥ 1 by specification */
    uint64_t un = (uint64_t)n;

    seq_t rad = rad_uint64 (un);
    seq_t m = (seq_t)un * rad + 1;
    return smallest_prime_factor_uint64 ((uint64_t)m);
}
