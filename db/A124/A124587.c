/*
 * A124587  Primes p such that q-p >= 16, where q is the next prime after p.
 * https://oeis.org/A124587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function computes the OEIS sequence A124587 – primes p such
 *            that the next prime q satisfies q − p ≥ 16
 * Range:     u8 none, u16 n <= 1304, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

/* Static cache for already-found qualifying primes. */
static seq_t *cache      = NULL;  /* array holding the qualifying primes */
static size_t cache_cap  = 0;     /* allocated capacity of the cache */
static idx_t   cache_len = 0;     /* number of elements currently stored */

/* Grow the cache to hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort(); /* out‑of‑memory: terminate the program */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* behaviour below the sequence offset is undefined */

    /* The last prime examined while filling the cache. */
    static uint64_t last_prime = 0; /* start with 0 so the first call gives 2 */

    /* Keep extending the cache until it contains at least `n` terms. */
    while (cache_len < n) {
        /* Find the next prime after the one we last inspected. */
        uint64_t p = next_prime(last_prime + 1);
        last_prime = p;

        /* Find the succeeding prime and test the gap. */
        uint64_t q = next_prime(p + 1);
        if (q - p >= 16) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
        /* If the gap is too small, simply continue searching. */
    }
    return cache[n - 1];
}
