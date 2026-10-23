/*
 * A141963  Primes congruent to 23 mod 27.
 * https://oeis.org/A141963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   This function returns the n-th prime congruent to 23 modulo 27
 *            (searching by trial primality testing)
 * Range:     u8 n <= 3, u16 n <= 370, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for size_t */

#include "../utils/isPrimeGeneric.h"

/* Static cache of computed primes */
static seq_t *cache = NULL;          /* array of primes found */
static size_t cache_size = 0;        /* number of elements stored */
static size_t cache_cap   = 0;        /* allocated capacity */

/* Candidate generation state (k such that candidate = 27*k+23). */
static idx_t next_k = 0;

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();   /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return the n-th prime ≡ 23 (mod 27). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, return 0 for safety */

    /* Generate primes until we have at least n terms. */
    while (cache_size < (size_t)n) {
        idx_t cand_idx = 27 * next_k + 23;
        seq_t cand = (seq_t)cand_idx;
        next_k++;

        if (isPrimeGeneric(cand)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = cand;
        }
    }
    return cache[n - 1];
}
