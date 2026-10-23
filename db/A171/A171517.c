/*
 * A171517  Primes p such that 2*p+11 is prime.
 * https://oeis.org/A171517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 904, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that 2*p + 11 is also prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"    /* is_prime(idx_t x) */

static seq_t *cache      = NULL;  /* cached sequence values     */
static size_t cache_cap  = 0;      /* allocated capacity         */
static idx_t  cache_len  = 0;      /* number of cached elements  */
static idx_t  current_candidate = 3;   /* next odd number to test    */

/* Ensure that cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Find next prime p such that 2*p+11 is prime. */
        for (;;) {
            if (is_prime(current_candidate)) {
                idx_t p = current_candidate;
                idx_t test = p * ((idx_t)2) + ((idx_t)11);
                if (is_prime(test)) {
                    /* Grow cache if necessary. */
                    if (cache_len >= cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) abort();   /* allocation failure */
                        cache     = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)p;
                    /* Advance to next odd candidate before resuming search. */
                    current_candidate += 2;
                    break;                /* found next term */
                }
            }
            /* Next odd candidate. */
            current_candidate += 2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, safe fallback */
    ensure_cache(n);
    return cache[n - 1];              /* 0‑based index → sequence element */
}
