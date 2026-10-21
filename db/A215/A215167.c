/*
 * A215167  Primes congruent to {2, 5} mod 11.
 * https://oeis.org/A215167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215167: a(n) = the n-th prime congruent to 2 or 5 modulo 11 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Generic dynamic array growth for seq_t arrays */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 32 : (*cap << 1);
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* Cached sequence values (index 0 => a(1)) */
static seq_t *cache    = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Next integer to examine for the prime condition */
static idx_t next_candidate = 2;

/* Return the n-th element of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */

    /* Build the cache up to the requested index */
    while (cache_size < (size_t)n) {
        idx_t cand = next_candidate++;

        /* Only consider candidates with the required congruence */
        if (cand % 11 != 2 && cand % 11 != 5) continue;

        /* Test primality using the provided big‑integer routine */
        if (is_prime_big((seq_t)cand)) {
            grow_seq_array(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
