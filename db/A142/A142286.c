/*
 * A142286  Primes congruent to 37 mod 43.
 * https://oeis.org/A142286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 154, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, is_prime_big, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A142286: primes congruent to 37 (mod 43). */
/* a(n) is the n‑th prime p with p ≡ 37 (mod 43), n ≥ 1. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */
#include "../utils/seq_free_array.h"    /* seq_free_array */

static seq_t *prime_cache = NULL;   /* array of found primes */
static idx_t   prime_cnt   = 0;     /* number of stored primes   */
static idx_t   candidate   = 37;    /* first number to test      */
static size_t cache_cap   = 0;      /* capacity of prime_cache  */

/* Grow the cache so it can hold at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (cache_cap >= (size_t)need)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < (size_t)need)
        new_cap <<= 1;

    seq_t *newp = alloc_seq_array(new_cap);
    for (idx_t i = 0; i < prime_cnt; ++i)
        newp[i] = prime_cache[i];

    if (prime_cache)
        seq_free_array(prime_cache);

    prime_cache = newp;
    cache_cap   = new_cap;
}

/* Return the n-th prime that is 37 (mod 43).  Undefined for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* out of range, keep behaviour benign */

    /* If we already have enough primes, just return the cached value. */
    if (n <= prime_cnt)
        return prime_cache[n - 1];

    /* Search for additional primes until we have at least n of them. */
    while (prime_cnt < n) {
        if (is_prime_big((seq_t)candidate)) {
            grow_cache(prime_cnt + 1);
            prime_cache[prime_cnt++] = (seq_t)candidate;
        }
        candidate += 43;
    }

    return prime_cache[n - 1];
}
