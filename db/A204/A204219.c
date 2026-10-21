/*
 * A204219  Primes whose binary reversal is not prime.
 * https://oeis.org/A204219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204219: a(n) = the nth prime p such that the binary
 * reversal of p is not prime.  The implementation searches forward
 * through primes, testing each reversed value with is_prime_big(). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start); */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

/* Reverse the binary representation of a 64‑bit integer.  The loop
 * stops when the original number has no more '1' bits, so leading
 * zeros are dropped. */
static uint64_t rev_bits(uint64_t n)
{
    uint64_t r = 0;
    while (n) {
        r = (r << 1) | (n & 1ULL);
        n >>= 1;
    }
    return r;
}

/* Cached sequence data. */
static seq_t *cache       = NULL;   /* array of qualifying primes */
static size_t cache_len   = 0;      /* number of cached elements   */
static size_t cache_cap   = 0;      /* current capacity            */
static uint64_t last_prime = 0;     /* last prime examined */

/* Grow the cache to hold at least 'needed' elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) abort();   /* OEIS offset is 1 – undefined for n=0 */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) {
        return cache[(size_t)n - 1];
    }

    /* Extend cache until it contains n elements. */
    while (cache_len < (size_t)n) {
        /* Find the next prime. */
        uint64_t p;
        if (last_prime == 0) {
            p = 2;
        } else {
            p = next_prime(last_prime + 1);
        }
        last_prime = p;

        /* Test the binary reversal for primality. */
        uint64_t rev = rev_bits(p);
        bool rev_is_prime = is_prime_big((seq_t)rev);

        if (!rev_is_prime) {
            /* Store the qualifying prime. */
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[(size_t)n - 1];
}
