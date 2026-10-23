/*
 * A132250  Isolated primes congruent to 13 (mod 30).
 * https://oeis.org/A132250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search for primes p ≡ 13 (mod 30) with both p − 2 and p + 2
 *            composite
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Static cache of computed terms (1‑based indexing). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* capacity of the cache array   */
    static idx_t last_candidate = 13;        /* last candidate checked */

    /* Allocate the cache on first use. */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* unlikely, but safe */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        last_candidate += 30;  /* next number ≡ 13 (mod 30) */

        /* Candidate must be prime and its neighbours ±2 must be composite. */
        if (is_prime(last_candidate) &&
            !is_prime(last_candidate - 2) &&
            !is_prime(last_candidate + 2)) {

            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;   /* double the capacity */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) { free(cache); return (seq_t)0; }
                cache = tmp;
            }

            /* Store the new term. */
            cache[cache_len++] = (seq_t)last_candidate;
        }
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
