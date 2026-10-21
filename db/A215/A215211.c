/*
 * A215211  Primes congruent to {2, 5, 7} mod 13.
 * https://oeis.org/A215211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 2 or 5 or 7 (mod 13)
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime congruent to 2, 5 or 7 modulo 13.
   It builds the values on demand and caches them so that repeated calls
   (in any order) are efficient. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *cache      = NULL;   /* stored primes */
    static size_t cache_len  = 0;      /* number of cached values */
    static size_t cache_cap  = 0;      /* current allocation cap */
    static uint64_t last_candidate = 1;/* last candidate examined */

    /* ensure cache grows to at least n entries */
    while (cache_len < (size_t)n) {
        /* extend cache capacity if needed */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0; /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* find next candidate prime with required residue */
        do {
            ++last_candidate;
        } while (!(last_candidate % 13ULL == 2ULL ||
                   last_candidate % 13ULL == 5ULL ||
                   last_candidate % 13ULL == 7ULL));

        seq_t cand_seq = (seq_t)last_candidate;
        if (isPrimeGeneric(cand_seq)) {
            cache[cache_len++] = cand_seq;
        }
    }

    return cache[n - 1];
}
