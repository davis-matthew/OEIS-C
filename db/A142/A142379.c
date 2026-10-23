/*
 * A142379  Primes congruent to 28 mod 47.
 * https://oeis.org/A142379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: collect primes congruent to 28 (mod 47) in increasing order.
   The sequence is the list of such primes, a(1)=263, a(2)=733, ... */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* cache of candidates found so far (stored as idx_t to keep allocation
   trivial in C++ for arbitrary‑precision seq_t). */
static idx_t *cache       = NULL;   /* array of candidate primes */
static size_t cache_size  = 0;      /* number of stored primes   */
static size_t cache_cap   = 0;      /* allocated capacity       */
static idx_t  last_k      = 4;      /* last multiplier used     */

/* Ensure the cache has room for 'need' items. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime congruent to 28 modulo 47 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n < 1 – harmless */

    /* Gather primes until we have at least n of them. */
    while (cache_size < (size_t)n) {
        ++last_k;                                       /* next candidate */
        idx_t cand = 47 * last_k + 28;                   /* number ≡28 (mod 47) */
        if (is_prime(cand)) {                           /* primality test   */
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = cand;                  /* cache the prime  */
        }
    }
    return (seq_t)cache[n - 1];
}
