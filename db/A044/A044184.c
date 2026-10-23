/*
 * A044184  Numbers n such that string 6,4 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the n-th positive integer whose base-7 representation contains the
 * substring "64" while the base-7 representation of n-1 does not.
 * The function caches previously found terms, so calls are
 * independent of the order of n.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has64.h"

/* Cached terms */
static seq_t      *terms = NULL;
static idx_t      terms_count = 0;
static size_t     terms_cap = 0;
/* Next candidate to test for the property */
static seq_t      next_candidate = 1;

/* Ensure the terms array has capacity for needed terms */
static void ensure_terms_capacity(idx_t needed) {
    if ((idx_t)terms_cap >= needed)
        return;
    size_t new_cap = terms_cap == 0 ? 8 : terms_cap;
    while ((idx_t)new_cap < needed)
        new_cap *= 2;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms)
        exit(1); /* allocation failed */
    terms = new_terms;
    terms_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) /* undefined for 0, return 0 as a safe value */
        return 0;

    while (terms_count < n) {
        /* Find next candidate satisfying the property */
        while (1) {
            seq_t cand = next_candidate;
            if (has64(cand) && !has64(cand - 1)) {
                /* Store the found term */
                ensure_terms_capacity(terms_count + 1);
                terms[terms_count++] = cand;
                /* Next candidate starts after this one */
                next_candidate = cand + 1;
                break;
            }
            next_candidate++;
        }
    }
    return terms[n - 1];
}
