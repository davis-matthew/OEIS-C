/*
 * A138410  a(n) = prime(n)^6 - prime(n)^3.
 * https://oeis.org/A138410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^6 - prime(n)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache for first k primes (1‑based indexing in a(n) uses n-1). */
static uint32_t *prime_cache = NULL;
static size_t prime_count = 0;

/* Ensure that prime_cache contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;                     /* already have enough */

    /* Allocate a new cache big enough for n primes. */
    uint32_t *new_cache = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!new_cache)
        return;                     /* allocation failed, leave unchanged */

    /* Copy the primes we already have. */
    for (size_t i = 0; i < prime_count; ++i)
        new_cache[i] = prime_cache[i];

    /* Start searching for new primes after the last one we know. */
    uint32_t candidate = (prime_count == 0) ? 2 : (prime_cache[prime_count - 1] + 1);

    while (prime_count < (size_t)n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            new_cache[prime_count++] = candidate;
        }
        ++candidate;
    }

    free(prime_cache);
    prime_cache = new_cache;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset is 1, behaviour for 0 is irrelevant */

    ensure_primes(n);                 /* guarantee we have the nth prime */

    seq_t p = (seq_t)prime_cache[n - 1];
    seq_t p3 = p * p * p;             /* p^3 */
    seq_t p6 = p3 * p3;               /* p^6 (p^3 * p^3) */
    return p6 - p3;                   /* prime^6 - prime^3 */
}
