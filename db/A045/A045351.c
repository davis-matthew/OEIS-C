/*
 * A045351  Primes congruent to {0, 2, 4, 5} mod 7.
 * https://oeis.org/A045351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find primes congruent to {0,2,4,5} mod 7; a(n) = the nth such prime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state for caching primes and results */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;

static uint64_t *prime_list = NULL; /* primes seen so far */
static size_t prime_len     = 0;
static size_t prime_cap     = 0;

static uint64_t last_checked = 1;

/* Ensure capacity of dynamic array */
static void ensure_cap(size_t *cap, size_t *len, void **data, size_t elem_size)
{
    if (*len < *cap) return;
    /* double the capacity, start from 8 */
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    void *new_data = realloc(*data, new_cap * elem_size);
    if (!new_data) abort();     /* out of memory */
    *data = new_data;
    *cap  = new_cap;
}

/* Test if n is prime using the primes found so far */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p) break;           /* p * p would overflow or exceed n */
        if (n % p == 0) return false;
    }
    return true;
}

/* Keep cache up to at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        uint64_t candidate = last_checked + 1;
        /* Skip even numbers greater than 2 for speed */
        if (candidate > 2 && (candidate & 1) == 0) {
            ++candidate;
        }

        /* Check primality */
        if (is_prime_uint64(candidate)) {
            /* Add candidate to prime list */
            ensure_cap(&prime_cap, &prime_len, (void **)&prime_list, sizeof(uint64_t));
            prime_list[prime_len++] = candidate;

            /* Accept if it satisfies the required residue condition */
            if (candidate == 7 || candidate % 7 == 2 || candidate % 7 == 4 ||
                candidate % 7 == 5) {
                ensure_cap(&cache_cap, &cache_len, (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = (seq_t)candidate;
            }
        }
        last_checked = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, value irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}
