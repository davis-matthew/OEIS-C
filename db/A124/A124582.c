/*
 * A124582  Primes p such that q-p >= 6, where q is the next prime after p.
 * https://oeis.org/A124582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A124582: primes p such that the next prime q satisfies q‑p ≥ 6. */
/* The function lazily builds the sequence and memoises it for arbitrary call order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/seq_realloc.h"  /* seq_t *seq_realloc(seq_t* old, size_t old_cap, size_t new_cap); */

seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1) is the first term. */
    if (n == 0) return 0;           /* harmless sentinel for out‑of‑range */
    
    /* Static state holding the generated terms. */
    static uint64_t last_prime = 2; /* prime previously examined */
    static seq_t *cache      = NULL;  /* array of qualifying primes */
    static size_t  cache_sz  = 0;     /* elements stored */
    static size_t  cache_cap = 0;     /* allocated capacity */
    
    /* Generate terms until we have at least n of them. */
    while (cache_sz < (size_t)n) {
        /* Find the next prime after the current one. */
        uint64_t next = next_prime(last_prime + 1);
        
        /* Store the current prime if the gap to the next one is at least 6. */
        if ((next - last_prime) >= 6) {
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) {   /* allocation failure – signal by returning 0 */
                    return 0;
                }
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_sz++] = (seq_t)last_prime;
        }
        /* Advance to the next prime and continue. */
        last_prime = next;
    }
    
    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
