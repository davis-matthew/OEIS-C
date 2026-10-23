/*
 * A103206  Concatenations of pairs of primes that differ by 6.
 * https://oeis.org/A103206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(A023201(n), A023201(n)+6) Here A023201(n) is the
 *            (n+1)-th prime p such that p+6 is also prime. The function
 *            precomputes enough such primes using a sieve and then
 *            concatenates the two numbers
 * Range:     u8 none, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10_digits.h"

static const uint32_t SIEVE_LIMIT = 20000000;   /* safe bound for >10k such primes */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Static arrays: sieve, prime list, and list of primes with +6 prime. */
    static bool *is_composite = NULL;
    static uint32_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static uint32_t *pair_primes = NULL;
    static size_t pair_cnt = 0;

    /* First call: allocate memory and build sieve. */
    if (!is_composite) {
        is_composite = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
        if (!is_composite) return 0;   /* allocation failed */

        /* Sieve of Eratosthenes */
        for (uint32_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
            if (!is_composite[i]) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }

        /* Extract primes */
        prime_cnt = 0;
        for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
            if (!is_composite[i]) prime_cnt++;

        prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
        if (!prime_list) return 0;
        size_t idx = 0;
        for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
            if (!is_composite[i]) prime_list[idx++] = i;

        /* Extract primes where p+6 is also prime (within limit). */
        size_t est_pairs = prime_cnt;  /* upper bound */
        pair_primes = (uint32_t*)malloc(est_pairs * sizeof(uint32_t));
        if (!pair_primes) return 0;
        pair_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if (p + 6 <= SIEVE_LIMIT && !is_composite[p + 6]) {
                pair_primes[pair_cnt++] = p;
            }
        }
    }

    /* Ensure we have enough pairs for the requested index. */
    if ((size_t)n >= pair_cnt) {
        /* In practice n will be < pair_cnt due to large sieve limit. */
        /* If not, return 0 to indicate failure. */
        return 0;
    }

    uint32_t p = pair_primes[(size_t)n];
    uint32_t q = p + 6;

    seq_t pow10 = pow10_digits(q);
    return (seq_t)p * pow10 + q;
}
