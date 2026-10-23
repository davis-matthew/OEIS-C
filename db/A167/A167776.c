/*
 * A167776  Composite numbers having six composite nearest-neighbors.
 * https://oeis.org/A167776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A167776: Composite numbers having six composite nearest‑neighbors
    (composite numbers whose three predecessors and three successors are all composite)
    Computation is performed by iterating over integers, testing primality,
    and collecting those that satisfy the neighbour condition.
    The function caches previously found terms so that calls with arbitrary
    order of n remain O(n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n) */

/* Global cache of known terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Last integer tested (inclusive) */
static idx_t last_tested = 3;

/* Grow terms storage to at least the provided capacity. */
static void grow_terms(size_t new_cap)
{
    if (terms_cap >= new_cap) return;
    size_t cap = terms_cap ? terms_cap << 1 : 16;
    while (cap < new_cap) cap <<= 1;
    terms = (seq_t *)realloc(terms, sizeof(seq_t) * cap);
    terms_cap = cap;
}

/* Check whether all six neighbours of n are composite.
   Numbers < 4 are not composite. */
static bool neighbours_composite(idx_t n)
{
    for (int k = -3; k <= 3; ++k) {
        if (k == 0) continue;
        idx_t m = n + (idx_t)k;
        if (m < 4u || is_prime(m)) return false;
    }
    return true;
}

/* Produce terms until we have at least 'need' entries in the cache. */
static void generate_terms_at_least(size_t need)
{
    if (terms_len >= need) return;
    /* Ensure storage */
    grow_terms(terms_len + need);

    /* Keep searching from the last checked number. */
    while (terms_len < need) {
        ++last_tested;
        if (is_prime(last_tested)) continue;          /* skip primes */
        if (neighbours_composite(last_tested)) {
            terms[terms_len++] = (seq_t)last_tested;
        }
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                     /* offset 1 only */
    generate_terms_at_least((size_t)n);
    return terms[(size_t)(n - 1)];
}
