/*
 * A123317  Smallest prime power m such that n+m is a prime number.
 * https://oeis.org/A123317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A123317: smallest prime power m such that n + m is prime.
   The function scans m = 1,2,3,…, returning the first prime power
   that makes n+m prime.  Prime powers are recognised by trial division
   on a pre‑computed list of primes up to 100 000, which covers all
   m that occur for n ≤ 10000 in under the 2‑second limit.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* Global storage for the prime list. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Initialise the prime list once. */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    /* 100 000 is comfortably larger than sqrt(max m) expected for n ≤ 10 000. */
    build_primes_upto(100000, &prime_list, &prime_count);
}

/* Test whether a uint64_t value is a prime power (1, p, p², …). */
static bool is_prime_power_uint64(uint64_t m)
{
    if (m == 1)          /* 1 is allowed as a prime power. */
        return true;

    uint64_t temp = m;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > temp / p)    /* p² > temp */
            break;
        if (temp % p == 0) {
            /* Divide out all factors of p. */
            while (temp % p == 0)
                temp /= p;
            /* We are a prime power iff nothing else remains. */
            return temp == 1;
        }
    }
    /* No small prime factor found – m is prime itself. */
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    /* Search for the smallest prime power m with n+m prime. */
    for (uint64_t m = 1; ; ++m) {
        if (!is_prime_power_uint64(m))
            continue;

        seq_t candidate = (seq_t)n + (seq_t)m;
        if (is_prime_big(candidate))
            return (seq_t)m;   /* m fits in seq_t because m is small. */
    }
}
