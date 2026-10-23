/*
 * A141908  Primes congruent to 2 mod 23.
 * https://oeis.org/A141908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 238, u32 n <= 239, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 2 mod 23 (search) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* allocated capacity */
static uint64_t next_candidate = 2;   /* next integer to test */

/* Ensure that the cache has at least 'need' slots. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least n terms. */
static void generate_terms_up_to(idx_t n)
{
    /* guard against unsigned wrap for n == 0 (undefined behaviour) */
    if (n == 0)
        return;

    while (cache_len < n) {
        /* Check modulo 23 and primality. */
        if ((next_candidate % 23ULL) == 2ULL && is_prime_big((seq_t)next_candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        /* Advance to the next candidate. */
        ++next_candidate;
    }
}

/* PUBLIC API --------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* The sequence is 1-indexed; behaviour for n < 1 is undefined. */
    generate_terms_up_to(n);
    return cache[n - 1];
}
