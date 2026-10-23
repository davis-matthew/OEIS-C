/*
 * A142289  Primes congruent to 40 mod 43.
 * https://oeis.org/A142289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 40 (mod 43)
 * Range:     u8 n <= 1, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Cache of found primes that are ≡40 (mod 43) */
static uint64_t *prime_cache   = NULL;
static size_t   prime_cache_len = 0;
static size_t   prime_cache_cap = 0;

/* Make sure the cache contains at least n primes. */
static void ensure_cache(idx_t n)
{
    if (prime_cache_len >= (size_t)n)
        return;

    /* Start from the next candidate in the residue class 40 (mod 43). */
    uint64_t candidate;
    if (prime_cache_len == 0)
        candidate = 40;                     /* will be skipped because 40 is composite */
    else
        candidate = prime_cache[prime_cache_len - 1] + 43;

    while (prime_cache_len < (size_t)n) {
        if (is_prime((idx_t)candidate)) {
            /* Grow the cache if needed before inserting. */
            if (prime_cache_len >= prime_cache_cap) {
                size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(EXIT_FAILURE);      /* unrecoverable allocation failure */
                prime_cache = tmp;
                prime_cache_cap = new_cap;
            }
            prime_cache[prime_cache_len++] = candidate;
        }
        candidate += 43; /* next number in the residue class */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n<1, but guard anyway */
        return 0;
    ensure_cache(n);
    return (seq_t)prime_cache[(size_t)n - 1];
}
