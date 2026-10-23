/*
 * A142398  Primes congruent to 1 mod 48.
 * https://oeis.org/A142398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 1 (mod 48)
 * Range:     u8 n <= 3, u16 n <= 395, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

static seq_t *prime_cache = NULL;   /* cached primes 1 mod 48          */
static size_t cache_len   = 0;       /* number of cached primes         */
static size_t cache_cap   = 0;       /* allocated capacity              */

/* Ensure the cache can hold at least 'new_len' primes. */
static void grow_cache(size_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* out of memory */
    prime_cache = new_cache;
    cache_cap   = new_cap;
}

/* Generate primes ≡ 1 (mod 48) until the cache holds at least 'n' primes. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    idx_t next_k = 2;                 /* first candidate 48*2+1 = 97   */
    if (cache_len > 0) {
        seq_t last_prime = prime_cache[cache_len - 1];
        /* last_prime fits in idx_t for the problem sizes considered */
        idx_t last_k = ((idx_t)last_prime - 1) / 48;
        next_k = last_k + 1;
    }

    while (cache_len < n) {
        idx_t candidate = 48 * next_k + 1;
        if (is_prime(candidate)) {
            grow_cache(cache_len + 1);
            prime_cache[cache_len++] = (seq_t)candidate;
        }
        ++next_k;
    }
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* Undefined for n = 0 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
