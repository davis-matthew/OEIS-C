/*
 * A230587  Number n such that the sum of its proper evil divisors (A001969)
 *          equals n.
 * https://oeis.org/A230587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 77, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sieve the sums of evil proper divisors.
   A number n is returned iff the sum of its proper evil divisors
   equals n (OEIS A230587).  The algorithm keeps a growing cache of
   partial sums up to the current limit and records the qualifying
   numbers in order.  The cache is updated only when a larger limit
   is required, making the function efficient for any sequence of
   calls regardless of their ordering. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static size_t max_checked = 0;          /* largest n already processed */

/* terms of the sequence, stored in ascending order.
   For C++ builds we use std::vector, for C builds a dynamically resized array. */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> terms;
#else
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
#endif

/* cache of sum of evil proper divisors for numbers up to max_checked.
   We store the sums in 64‑bit unsigned integers – this is sufficient for
   all values that can appear in the sequence when seq_t is 64‑bit or larger. */
static uint64_t *sum_cache = NULL;
static size_t sum_cap = 0;

/* append a new term to the sequence list */
static void push_term(seq_t val) {
#ifdef __cplusplus
    terms.push_back(val);
#else
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return;                     /* allocation failure – give up */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
#endif
}

/* extend the cache up to new_limit (inclusive).  The function calculates
   contributions of all evil divisors to numbers in the new range and
   records any numbers whose evil divisor sum equals the number itself. */
static void extend_cache(size_t new_limit) {
    if (new_limit <= max_checked) return;

    /* resize sum_cache */
    size_t old_limit = max_checked;
    uint64_t *new_cache = (uint64_t *)realloc(sum_cache,
                                               (new_limit + 1) * sizeof(uint64_t));
    if (!new_cache) return;                    /* allocation failure – give up */
    sum_cache = new_cache;
    /* zero the newly added entries */
    memset(sum_cache + old_limit + 1,
           0,
           (new_limit - old_limit) * sizeof(uint64_t));

    /* 1. add contributions of divisors <= old_limit to numbers > old_limit */
    for (uint64_t d = 1; d <= old_limit; ++d) {
        if ((__builtin_popcountll(d) & 1) == 0) {          /* d is evil */
            uint64_t start = ((old_limit + 1 + d - 1) / d) * d;
            for (uint64_t m = start; m <= new_limit; m += d) {
                sum_cache[m] += d;
            }
        }
    }

    /* 2. add contributions of new divisors (old_limit < d <= new_limit) */
    for (uint64_t d = old_limit + 1; d <= new_limit; ++d) {
        if ((__builtin_popcountll(d) & 1) == 0) {          /* d is evil */
            for (uint64_t m = 2 * d; m <= new_limit; m += d) {
                sum_cache[m] += d;
            }
        }
    }

    /* 3. record numbers that satisfy the condition in the new range */
    for (uint64_t m = old_limit + 1; m <= new_limit; ++m) {
        if (sum_cache[m] == m) {
            push_term((seq_t)m);
        }
    }

    max_checked = new_limit;
    sum_cap = new_limit + 1;
}

/* Main function required by the task spec. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined index – protect */

    /* Ensure we have at least n terms in the sequence list. */
    while (1) {
#ifdef __cplusplus
        size_t terms_needed = (size_t)n;
        if (terms.size() >= terms_needed) break;
        /* heuristic to enlarge the search: double the current limit */
        size_t new_limit = max_checked > 0 ? max_checked * 2 : 256;
        if (new_limit < max_checked + 1) new_limit = max_checked + 1;
        extend_cache(new_limit);
#else
        size_t terms_needed = (size_t)n;
        if (terms_len >= terms_needed) break;
        size_t new_limit = max_checked > 0 ? max_checked * 2 : 256;
        if (new_limit < max_checked + 1) new_limit = max_checked + 1;
        extend_cache(new_limit);
#endif
    }

#ifdef __cplusplus
    return terms[(size_t)(n - 1)];
#else
    return terms[n - 1];
#endif
}
