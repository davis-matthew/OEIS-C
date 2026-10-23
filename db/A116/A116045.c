/*
 * A116045  n+phi(n)+phi(phi(n)) is a prime.
 * https://oeis.org/A116045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A116045
   a(n) = the n‑th integer m such that m + φ(m) + φ(φ(m)) is prime.
   Indexing starts at 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"

/* Helper: compute φ(m) for a seq_t value using the provided utility. */
static seq_t phi(seq_t m) { return calc_phi(m); }

/* One static cache that persists across calls. */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t next_candidate = 1;  /* next integer to test */

seq_t a(idx_t n) {
    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t m = (seq_t)next_candidate;
        seq_t phi1 = phi(m);
        seq_t phi2 = phi(phi1);

        /* sum = m + phi(m) + phi(phi(m)) */
        seq_t sum = m + phi1 + phi2;
        if (is_prime_big(sum)) {
            /* Ensure capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        next_candidate++;                /* advance to next integer */
    }
    return cache[n - 1];
}
