/*
 * A224534  Prime numbers that are the sum of three distinct prime numbers.
 * https://oeis.org/A224534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A224534 is the set of all primes ≥ 19. Thus a(n) is the (n+7)-th
 *            prime (the 8th prime is 19)
 * Range:     u8 n <= 47, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility for primality testing of 64‑bit numbers. */
#include "../utils/is_prime_small.h"

/* ---------- Prime cache implementation ---------- */

static uint64_t *prime_cache   = NULL;  /* array of primes (uint64_t) */
static size_t   prime_count   = 0;     /* number of primes stored   */
static size_t   prime_capacity = 0;    /* allocated size of array    */

/* Ensure that prime_cache contains at least need primes. */
static void
ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* Grow the cache if needed.  Initial capacity is 16. */
    if (prime_capacity == 0) {
        prime_capacity = 16;
        prime_cache    = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
        if (!prime_cache) abort();
    }
    while (prime_count < need) {
        /* Increase capacity if necessary. */
        if (prime_count == prime_capacity) {
            prime_capacity *= 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_cache, prime_capacity * sizeof(uint64_t));
            if (!tmp) abort();
            prime_cache = tmp;
        }

        /* Find next prime. */
        uint64_t candidate;
        if (prime_count == 0)
            candidate = 2;
        else if (prime_cache[prime_count - 1] == 2)
            candidate = 3;
        else
            candidate = prime_cache[prime_count - 1] + 2; /* skip even numbers */

        while (!is_prime_small(candidate)) {
            candidate += 2;
        }
        prime_cache[prime_count++] = candidate;
    }
}

/* ---------- Sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for indices < 1 */
        return (seq_t)0;

    /* We need the (n+7)-th prime. */
    size_t need = (size_t)(n + 7);
    ensure_primes(need);

    return (seq_t)prime_cache[need - 1];
}
