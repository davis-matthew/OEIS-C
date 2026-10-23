/*
 * A141986  Primes congruent to 10 mod 29.
 * https://oeis.org/A141986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 206, u32 n <= 207, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 10 modulo 29.
   a(1) = 97, a(2) = 271, … */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes that are 10 mod 29.
       The cache holds 64‑bit values; they are converted to seq_t when returned. */
    static uint64_t *cache = NULL;
    static idx_t  count  = 0;      /* number of cached values */
    static size_t cap    = 0;      /* allocated capacity of cache */
    static uint64_t next_k = 3;    /* k such that 29*k+10 is the next candidate */

    /* Grow the cache until we have at least n primes. */
    while (count < n) {
        uint64_t cand = 29 * next_k + 10;   /* candidate ≡ 10 (mod 29) */
        if (is_prime_big((seq_t)cand)) {
            /* Allocate or grow the cache. */
            if (count == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                uint64_t *new_cache = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
                if (!new_cache) {
                    /* Out of memory – abort. */
                    exit(EXIT_FAILURE);
                }
                /* Copy existing entries. */
                for (idx_t i = 0; i < count; ++i) {
                    new_cache[i] = cache[i];
                }
                /* Free old storage. */
                free(cache);
                cache = new_cache;
                cap   = new_cap;
            }
            cache[count++] = cand;
        }
        ++next_k;
    }
    return (seq_t)cache[n - 1];   /* 0‑based indexing into the cache */
}
