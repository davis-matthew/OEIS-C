/*
 * A089124  Largest prime factor of n^2 + 3.
 * https://oeis.org/A089124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime factor of n^2 + 3 (OEIS A089124)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define PRIME_LIMIT 20000       /* > sqrt(10000^2+3) */

/* Prime table (generated once). */
static uint32_t primes[PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_initialized = false;

/* Simple sieve of Eratosthenes to fill the prime table. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    bool composite[PRIME_LIMIT];
    for (size_t i = 0; i < PRIME_LIMIT; ++i)
        composite[i] = false;

    for (uint32_t p = 2; p < PRIME_LIMIT; ++p) {
        if (!composite[p]) {
            primes[prime_count++] = p;
            if ((uint64_t)p * p < PRIME_LIMIT) {
                for (uint32_t q = p * p; q < PRIME_LIMIT; q += p)
                    composite[q] = true;
            }
        }
    }
    primes_initialized = true;
}

/* Return the largest prime factor of n^2 + 3. */
seq_t a(idx_t n)
{
    init_primes();

    seq_t m = (seq_t)n * (seq_t)n + (seq_t)3;
    seq_t largest = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p2 = p * p;
        if (p2 > m)
            break;

        if (m % p == 0) {
            largest = p;
            while (m % p == 0)
                m /= p;
        }
    }

    if (m > 1)          /* Remaining factor is prime and larger than all seen. */
        largest = m;

    return largest;
}
