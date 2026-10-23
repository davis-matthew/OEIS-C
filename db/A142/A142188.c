/*
 * A142188  Primes congruent to 13 mod 40.
 * https://oeis.org/A142188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 13 (mod 40)
 * Range:     u8 n <= 3, u16 n <= 419, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Internal cache of found primes */
static seq_t   *prime_cache = NULL;
static size_t   cache_cap   = 0;
static size_t   cache_len   = 0;
static idx_t    last_candidate = 13;   /* 13 is the first prime of the form 40k+13 */

/* Ensure that at least 'needed' primes are cached. */
static void ensure_primes(idx_t needed)
{
    if (needed == 0) return;

    if (!prime_cache) {
        cache_cap = 1024;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)needed) {
        if (is_prime(last_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                prime_cache = (seq_t *)realloc(prime_cache,
                                               cache_cap * sizeof(seq_t));
            }
            prime_cache[cache_len++] = (seq_t)last_candidate;
        }
        last_candidate += 40;
    }
}

/* Public function required by the interface. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* offset is 1; behaviour below is unspecified */
    ensure_primes(n);
    return prime_cache[n - 1];
}
