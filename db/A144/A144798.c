/*
 * A144798  Integers having ideal digital mean up to base 3.
 * https://oeis.org/A144798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1272, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9333 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Return true if x has an equal number of zeros and ones in its
 * binary representation. */
static bool binary_ok(seq_t x) {
    if (x == 0) return false;
    seq_t t = x;
    unsigned long long ones = 0, d = 0;
    while (t != 0) {
        ones += (uint64_t)(t & 1);
        t >>= 1;
        d++;
    }
    return (d & 1) == 0 && ones == d / 2;
}

/* Return true if x has an equal number of zeros and twos in its
 * base‑3 representation. */
static bool ternary_ok(seq_t x) {
    if (x == 0) return false;
    seq_t t = x;
    unsigned long long zeros = 0, twos = 0;
    while (t != 0) {
        uint64_t digit = (uint64_t)(t % 3);
        if (digit == 0) zeros++;
        else if (digit == 2) twos++;
        t /= 3;
    }
    return zeros == twos;
}

/* Return true iff x is a term of A144798. */
static bool term_valid(seq_t x) {
    return binary_ok(x) && ternary_ok(x);
}

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
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
