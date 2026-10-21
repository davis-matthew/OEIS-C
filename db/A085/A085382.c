/*
 * A085382  Sum of p = prime(n) and largest prime divisor of p-1.
 * https://oeis.org/A085382
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + largest_prime_divisor(prime(n) - 1)
 * Range:     u8 n <= 40, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 2; values for n < 2 are returned as 0. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/largest_prime_divisor.h" /* provides seq_t largest_prime_divisor(uint64_t) */

/* Return the nth prime (1‑based).  The list of primes is built on demand
   and cached in static storage so that calls can be in any order. */
static uint64_t get_prime(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t primes_count = 0;
    static size_t primes_cap   = 0;

    if (n <= primes_count)
        return primes[n - 1];

    uint64_t candidate = (primes_count == 0) ? 2ULL : primes[primes_count - 1] + 1ULL;

    while (primes_count < n)
    {
        bool is_prime = true;

        for (size_t i = 0; i < primes_count; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0ULL)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            if (primes_count >= primes_cap)
            {
                primes_cap = primes_cap == 0 ? 128 : primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[primes_count++] = candidate;
        }

        ++candidate;
    }

    return primes[n - 1];
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    uint64_t p   = get_prime(n);                         /* nth prime              */
    seq_t   lpf  = largest_prime_divisor(p - 1ULL);      /* largest prime factor of p-1 */
    return (seq_t)p + lpf;
}
