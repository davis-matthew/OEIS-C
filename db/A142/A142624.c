/*
 * A142624  Primes congruent to 32 mod 55.
 * https://oeis.org/A142624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 137, u32 n <= 138, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime numbers congruent to 32 modulo 55.
   a(n) is the n-th prime p such that p % 55 == 32, for n ≥ 1. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Static cache of found terms, growing as needed. */
static seq_t *prime_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* The next candidate number that is ≡ 32 (mod 55). */
static seq_t next_candidate = 32;

/* Double the cache capacity. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();          /* allocation failed */
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime ≡ 32 (mod 55). */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; return 0 for safety */
        return 0;

    /* Generate candidates until we have at least n terms. */
    while (cache_len < n) {
        if (is_prime_big(next_candidate)) {
            if (cache_len == cache_cap)
                grow_cache();
            prime_cache[cache_len++] = next_candidate;
        }
        /* Advance to the next 32‑mod‑55 candidate. */
        next_candidate += (seq_t)55;
    }

    return prime_cache[n - 1];
}
