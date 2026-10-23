/*
 * A167311  Totally multiplicative sequence with a(p) = 10*(p+2) for prime p.
 * https://oeis.org/A167311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (10 · (p+2))^e
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the totally multiplicative sequence A167311 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Small sieve up to 10000, enough for n ≤ 10000. */
    static bool initialized = false;
    static uint32_t primes[2000];
    static size_t prime_count = 0;
    if (!initialized)
    {
        const idx_t LIMIT = 10000;
        bool is_prime[LIMIT + 1];
        for (idx_t i = 2; i <= LIMIT; ++i)
            is_prime[i] = true;
        for (idx_t i = 2; i * i <= LIMIT; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = false;
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i])
                primes[prime_count++] = (uint32_t)i;
        initialized = true;
    }

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp)
            break;
        if (temp % p == 0)
        {
            idx_t exp = 0;
            do
            {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            /* compute (10·(p+2))^exp */
            seq_t base = (seq_t)10 * ((seq_t)(p + 2));
            seq_t factor = (seq_t)1;
            for (idx_t k = 0; k < exp; ++k)
                factor *= base;
            result *= factor;
        }
    }

    if (temp > 1) /* remaining prime factor */
    {
        uint32_t p = (uint32_t)temp;
        seq_t base = (seq_t)10 * ((seq_t)(p + 2));
        result *= base; /* exponent is 1 */
    }

    return result;
}
