/*
 * A144798  Integers having ideal digital mean up to base 3.
 * https://oeis.org/A144798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1272, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binary_ok, term_valid, ternary_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144798: integers having an equal number of 0's and 1's in
 * binary representation and an equal number of 0's and 2's in
 * ternary representation.  This implementation searches integers
 * sequentially and caches results. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binary_ok.h"
#include "../utils/term_valid.h"
#include "../utils/ternary_ok.h"

/* Cache of found terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of stored terms   */
static size_t cache_cap  = 0;   /* allocated capacity       */
static seq_t next_candidate = 1; /* next integer to test     */

/* Ensure at least n terms are generated and stored. */
static void ensure_terms(idx_t n) {
    if (cache_size >= (size_t)n) return;

    /* Grow cache if necessary. */
    size_t needed = (size_t)n;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have n of them. */
    while (cache_size < needed) {
        if (term_valid(next_candidate)) {
            cache[cache_size++] = next_candidate;
        }
        next_candidate++;
    }
}

/* Required function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined; safe return */
    ensure_terms(n);
    return cache[n - 1];
}
