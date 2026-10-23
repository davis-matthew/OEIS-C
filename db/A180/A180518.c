/*
 * A180518  Primes that become a different prime under the mapping 0 <=> 3.
 * https://oeis.org/A180518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 900, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, map_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime satisfying the property:
   after swapping all decimal digits 0 and 3 the resulting number is a
   different prime.  The implementation builds the sequence on demand
   and caches the found values.  It works for any seq_t (fixed‑width
   or arbitrary precision) and any idx_t (uint64_t). */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/is_prime_big.h"
#include "../utils/map_digits.h"

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
/* Current candidate for prime testing, kept as seq_t for arbitrary‑precision */
static seq_t next_candidate = 2;

seq_t a(idx_t n)
{
    if (n == 0)          /* no definition for n=0; return 0 as sentinel */
        return 0;

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Jump to next prime candidate */
        while (!is_prime_big(next_candidate))
            next_candidate++;

        /* Check mapping property */
        seq_t mapped = map_digits(next_candidate);
        if (mapped != next_candidate && is_prime_big(mapped)) {
            /* Ensure cache has space */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                /* realloc with explicit cast for C++ compatibility */
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }

        /* Advance to the next integer for the next iteration */
        next_candidate++;
    }

    return cache[n - 1];
}
