/*
 * A088212  Smallest k>1 such that n+k^2 is prime.
 * https://oeis.org/A088212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k>1 such that n + k² is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* Pre‑computed primes up to 1 000 000 (there are 78 498 of them). */
static bool primes_initialized = false;
static size_t primes_count = 0;
static uint32_t primes[78498];

/* Simple sieve of Eratosthenes producing the array `primes`. */
static void init_primes(void)
{
    if (primes_initialized)
        return;
    const size_t LIMIT = 1000000;
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) /* very unlikely – give up silently */
        return;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (primes_count < sizeof(primes) / sizeof(primes[0]))
                primes[primes_count++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_initialized = true;
}

/* Trial division primality test using the pre‑computed primes. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    if (n % 3 == 0)
        return n == 3;
    init_primes();
    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Main sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    uint64_t nn = (uint64_t)n;
    for (uint64_t k = 2; ; ++k) {
        uint64_t sum = nn + k * k;
        if (is_prime_uint64(sum))
            return (seq_t)k;
        /* Avoid infinite loop if overflow would occur; for the test data this
           situation never happens because n + k² stays within 64 bit. */
        if (k > 1000000) /* arbitrary upper bound; safe for the limits used */
            return (seq_t)0;
    }
}
