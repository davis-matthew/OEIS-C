/*
 * A140587  Primes congruent to 31 or 179 mod 210.
 * https://oeis.org/A140587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 31 or 179 modulo 210 (OEIS A140587). */
/* This implementation builds the sequence on demand, extending a
 * static cache. It uses the helper `is_prime(idx_t)` to test primality
 * and standard malloc/realloc for storage. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x); */

static seq_t *prime_cache = NULL;   /* cache of sequence terms          */
static size_t  cache_len  = 0;       /* number of elements currently stored */
static size_t  cache_cap  = 0;       /* current capacity of the cache     */
static idx_t   last_checked = 1;     /* last integer that was tested      */

/* Extend the cache so that it contains at least `n` terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;               /* already have enough */

    /* Allocate initial capacity if needed. */
    if (!prime_cache) {
        cache_cap      = 64;
        prime_cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!prime_cache) return; /* allocation failure – return (caller will handle). */
        cache_len = 0;
    }

    /* Start testing from the next integer after the last one we checked. */
    idx_t cand = last_checked + 1;
    if (cand < 2) cand = 2;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Skip numbers that cannot be 31 or 179 mod 210. */
        idx_t r = cand % 210;
        if (r == 31 || r == 179) {
            if (is_prime(cand)) { /* primality test */
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                    if (!tmp) return; /* allocation failure – return */
                    prime_cache = tmp;
                    cache_cap   = new_cap;
                }
                prime_cache[cache_len++] = (seq_t)cand;
            }
        }
        ++cand;
    }

    last_checked = cand - 1;   /* remember progress */
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for non‑positive indices */
    ensure_cache(n);
    return prime_cache[n - 1];
}
