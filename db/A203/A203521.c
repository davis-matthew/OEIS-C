/*
 * A203521  a(n) = Product_{1 <= i < j <= n} (prime(i) + prime(j)).
 * https://oeis.org/A203521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{1 <= i < j <= n} (prime(i) + prime(j))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: generate at least n primes, keep them in a static array.
   primes_cache[i] holds the (i+1)-th prime. */
static uint32_t *primes_cache = NULL;
static idx_t primes_cache_count = 0;

static void ensure_primes(idx_t n)
{
    if (n <= primes_cache_count) return;

    /* Reallocate cache */
    uint32_t *new_cache = (uint32_t *)realloc(primes_cache, n * sizeof(uint32_t));
    if (!new_cache) return; /* allocation failed – just exit silently */
    primes_cache = new_cache;

    /* Start generating new primes, beginning after the last one we have */
    uint32_t start = 2;
    if (primes_cache_count > 0)
        start = primes_cache[primes_cache_count - 1] + 1;

    idx_t count = primes_cache_count;
    for (uint32_t cand = start; count < n; ++cand)
    {
        bool is_prime = true;
        /* Test divisibility only by primes <= sqrt(cand) */
        for (idx_t pi = 0; pi < count; ++pi)
        {
            uint32_t p = primes_cache[pi];
            if ((uint64_t)p * (uint64_t)p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime)
            primes_cache[count++] = cand;
    }
    primes_cache_count = n;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(0) = a(1) = 1 */

    ensure_primes(n);                     /* make sure we have the first n primes */

    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
    {
        uint32_t pi = primes_cache[i];
        for (idx_t j = i + 1; j < n; ++j)
        {
            uint32_t pj = primes_cache[j];
            result *= ((seq_t)pi + (seq_t)pj);
        }
    }
    return result;
}
