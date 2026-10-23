/*
 * A179802  Digital root of A179545.
 * https://oeis.org/A179802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 3 if prime(n) ≡ 2 (mod 3); otherwise a(n) = 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* a(0) (outside the OEIS offset) is defined as 0 for safety. */
    if (n < 1)
        return (seq_t)0;

    /* Static storage for the primes discovered so far. */
    static uint32_t *primes = NULL;   /* array of found primes */
    static size_t prime_cnt = 0;      /* number of primes stored   */
    static size_t prime_cap = 0;      /* current capacity of the array */

    /* Grow the list until we have the n‑th prime. */
    if (prime_cnt < (size_t)n)
    {
        uint64_t candidate = (prime_cnt == 0) ? 2ULL : (uint64_t)primes[prime_cnt - 1] + 1ULL;

        while (prime_cnt < (size_t)n)
        {
            bool is_prime = true;

            /* Trial division by known primes up to sqrt(candidate). */
            for (size_t i = 0; i < prime_cnt &&
                 (uint64_t)primes[i] * primes[i] <= candidate; ++i)
            {
                if (candidate % primes[i] == 0)
                {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime)
            {
                /* Ensure we have room in the array. */
                if (prime_cnt == prime_cap)
                {
                    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                    if (!tmp)
                        return (seq_t)0; /* allocation failed */
                    primes = tmp;
                    prime_cap = new_cap;
                }
                primes[prime_cnt++] = (uint32_t)candidate;
            }
            ++candidate;
        }
    }

    /* primes[n-1] is the nth prime. Decide the digital root. */
    return (primes[n - 1] % 3 == 2) ? (seq_t)3 : (seq_t)9;
}
