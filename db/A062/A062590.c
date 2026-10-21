/*
 * A062590  Variation on A029834: a discrete version of the Mangoldt function.
 *          If n is prime then floor(log(prime(n))) else 0.
 * https://oeis.org/A062590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log(prime(n))) if n is prime; otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log_e, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_log_e.h"

/* static storage for generated primes (0-indexed) */
static uint64_t *primes = NULL;
static size_t primes_alloc = 0;
static size_t primes_count = 0;
static uint64_t next_candidate = 2;

/* generate primes up to at least the k-th (1‑based) prime */
static void _ensure_primes(size_t k)
{
    if (k <= primes_count) return;

    if (!primes) {
        primes_alloc = 64;
        primes = (uint64_t *)malloc(primes_alloc * sizeof(uint64_t));
    }

    while (primes_count < k) {
        uint64_t c = next_candidate;
        bool prime_candidate = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > c) break;
            if (c % p == 0) { prime_candidate = false; break; }
        }
        while (!prime_candidate) {
            ++c;
            prime_candidate = true;
            for (size_t i = 0; i < primes_count; ++i) {
                uint64_t p = primes[i];
                if (p * p > c) break;
                if (c % p == 0) { prime_candidate = false; break; }
            }
        }
        if (primes_count >= primes_alloc) {
            primes_alloc *= 2;
            primes = (uint64_t *)realloc(primes, primes_alloc * sizeof(uint64_t));
        }
        primes[primes_count++] = c;
        next_candidate = c + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!is_prime(n)) return 0;

    _ensure_primes((size_t)n);

    uint64_t p = primes[n - 1];
    return (seq_t)floor_log_e(p);
}
