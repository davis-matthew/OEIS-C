/*
 * A100350  Primes p such that p-2^k is a prime or semiprime for all k > 0
 *          with 2^k < p.
 * https://oeis.org/A100350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that for every k>0 with 2^k < p,
 * the number p-2^k is a prime or a semiprime.  The function
 * searches successive primes, testing the required property by
 * using a fast primality test and a simple semiprime recogniser
 * based on trial division with a pre‑computed prime list up to
 * 1 000 000.  It is fully compliant with C11 and C++17. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

#define SIEVE_LIMIT 1000000UL   /* primes up to sqrt(10^12) are enough */

static uint32_t prime_list[SIEVE_LIMIT + 1];
static size_t  prime_count = 0;
static bool    primes_initialised = false;

/* Initialise the prime list (once) */
static void init_primes(void)
{
    if (primes_initialised) return;
    bool sieve[(size_t)SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) sieve[i] = false;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = true;
        }
    }

    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            prime_list[idx++] = (uint32_t)i;
        }
    }
    prime_count = idx;
    primes_initialised = true;
}

/* Test whether n is a semiprime (product of exactly two primes,
 * counting multiplicity).  Uses the pre‑computed prime list for
 * trial division. */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;                 /* 2*2 is the smallest */
    init_primes();

    uint64_t tmp = n;
    unsigned factors = 0;

    for (size_t i = 0; i < prime_count &&
          (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i)
    {
        uint32_t p = prime_list[i];
        while (tmp % p == 0) {
            ++factors;
            tmp /= p;
            if (factors > 2) return false;
        }
    }
    if (tmp > 1) ++factors;   /* remaining prime factor > sqrt(n) */
    return factors == 2;
}

/* Check the defining property for a candidate prime p. */
static bool valid_prime(uint64_t p)
{
    /* Iterate over powers of two: 2^k < p */
    for (uint64_t pow = 2; pow < p; ) {
        uint64_t m = p - pow;
        if (!(is_prime_uint64(m) || is_semiprime(m)))
            return false;
        if (pow > UINT64_MAX / 2) break;   /* avoid overflow */
        pow <<= 1;
    }
    return true;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, safe guard */

    size_t need = (size_t)n;
    size_t count = 0;
    uint64_t candidate = 3;        /* first odd prime */

    while (count < need) {
        if (is_prime_uint64(candidate) && valid_prime(candidate)) {
            ++count;
            if (count == need) return (seq_t)candidate;
        }
        candidate += 2;            /* skip even numbers */
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
