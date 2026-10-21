/*
 * A052369  Largest prime factor of n, where n runs through composite numbers.
 * https://oeis.org/A052369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest_prime_factor(m) where m is the n-th composite
 *            integer >= 4
 * Range:     u8 n <= 415, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define LIMIT 200000  /* sufficient for n up to ~200k; adjust if larger n are needed */

static uint32_t lpf[LIMIT + 1];
static uint32_t primes[220000];  /* enough primes up to LIMIT */
static size_t primes_cnt = 0;
static uint32_t composites[LIMIT];
static size_t composites_cnt = 0;
static bool initialized = false;

static void init(void)
{
    /* Sieve of Eratosthenes storing the largest prime factor for each number */
    for (size_t i = 0; i <= LIMIT; ++i)
        lpf[i] = 0;

    for (size_t p = 2; p <= LIMIT; ++p) {
        if (lpf[p] == 0) {        /* p is prime */
            primes[primes_cnt++] = (uint32_t)p;
            for (size_t j = p; j <= LIMIT; j += p)
                lpf[j] = (uint32_t)p;   /* later (larger) primes overwrite */
        }
    }

    /* Collect all composite numbers ≥ 4 */
    composites_cnt = 0;
    for (size_t i = 4; i <= LIMIT; ++i) {
        if (lpf[i] != (uint32_t)i)   /* not a prime */
            composites[composites_cnt++] = (uint32_t)i;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n == 0 || (size_t)(n - 1) >= composites_cnt)
        return (seq_t)0;   /* Undefined or out of precomputed range */

    uint32_t val = composites[n - 1];
    return (seq_t)lpf[val];
}
