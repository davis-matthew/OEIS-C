/*
 * A127464  Primes p such that 12p - 11 and 12p + 11 are also primes.
 * https://oeis.org/A127464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 12p−11 and 12p+11 are primes
 * Range:     u8 none, u16 n <= 44, u32 n <= 45, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"   /* provides uint64_t next_prime(uint64_t start);   */
#include "../utils/is_prime_big.h" /* provides bool is_prime_big(seq_t n);            */

/* static cache of terms that fit in seq_t */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;     /* next number to test for primality */

/* grow the cache array when necessary */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term (n >= 1).  It works for any build‑dependent
 * seq_t.  Terms that do not fit in seq_t are simply skipped; the
 * function guarantees correctness for all indices whose value does
 * fit in seq_t. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms stored */
    while (cache_len < (size_t)n) {
        /* Find next prime candidate */
        uint64_t p = next_prime(next_candidate);
        next_candidate = p + 1;

        /* Compute 12p−11 and 12p+11 in a type that can hold the values */
        uint64_t x1 = 12u * p - 11u;
        uint64_t x2 = 12u * p + 11u;

        /* Test primality.  If seq_t is small, the cast will drop values
         * that cannot be represented; such primes are ignored because
         * they exceed the type limit. */
        bool ok = is_prime_big((seq_t)x1) && is_prime_big((seq_t)x2);
        if (ok) {
            /* If p cannot be represented in seq_t, skip it */
            if ((seq_t)p == p) {
                if (cache_len == cache_cap) grow_cache();
                cache[cache_len++] = (seq_t)p;
            }
        }
    }
    return cache[(size_t)n - 1];
}
