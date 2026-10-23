/*
 * A142374  Primes congruent to 23 mod 47.
 * https://oeis.org/A142374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   n-th prime congruent to 23 modulo 47, found by sequential search
 * Range:     u8 n <= 2, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;
static size_t prime_cache_size = 0;
static size_t prime_cache_cap = 0;
static uint64_t last_checked = 0;

/* Ensure the cache has at least one slot; double the capacity when needed. */
static void grow_prime_cache(void)
{
    if (prime_cache_cap == 0) {
        prime_cache_cap = 64;
        prime_cache = (seq_t *)calloc(prime_cache_cap, sizeof(seq_t));
    } else {
        prime_cache_cap <<= 1;
        prime_cache = (seq_t *)realloc(prime_cache, prime_cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1; lower indices are undefined  */
    /* Initialise cache on first call. */
    if (prime_cache == NULL) {
        grow_prime_cache();
        prime_cache[0] = (seq_t)23;
        prime_cache_size = 1;
        last_checked = 23;
    }
    /* Generate further primes until we reach the requested index. */
    while (prime_cache_size < n) {
        uint64_t candidate = last_checked + 47;
        while (!is_prime(candidate))
            candidate += 47;   /* keep the same remainder modulo 47            */
        if (prime_cache_size == prime_cache_cap)
            grow_prime_cache();
        prime_cache[prime_cache_size++] = (seq_t)candidate;
        last_checked = candidate;
    }
    return prime_cache[n - 1];
}
