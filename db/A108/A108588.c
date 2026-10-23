/*
 * A108588  Numbers k such that 10*k + 97 is prime.
 * https://oeis.org/A108588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108588: numbers k such that 10*k + 97 is prime.
   a(n) = the n‑th non‑negative integer k for which 10*k+97 is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve for numbers up to sqrt(10*k+97).  For n≈10000 the
   corresponding k is below 6 000 000, so 10*k+97 < 60 000 000.
   sqrt(60 000 000) ≈ 7 700, so 3500 is plenty. */
static const uint32_t PRIME_LIMIT = 3500;
static uint32_t primes[400];
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    bool is_composite[PRIME_LIMIT + 1];
    for (uint32_t i = 0; i <= PRIME_LIMIT; ++i)
        is_composite[i] = false;
    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = true;
    prime_cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            primes[prime_cnt++] = i;
    primes_initialized = true;
}

static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    if (!primes_initialized) init_primes();
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* iterate through k starting from 0, counting primes of the form 10*k+97 */
    idx_t count = 0;
    idx_t k    = 0;

    while (true) {
        uint64_t candidate = (uint64_t)k * 10ULL + 97ULL;
        if (is_prime_uint64(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
