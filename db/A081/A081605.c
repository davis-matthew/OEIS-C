/*
 * A081605  Numbers having at least one 0 in their ternary representation.
 * https://oeis.org/A081605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_in_ternary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates the numbers whose ternary representation contains at least one digit 0.
   a(1) = 0, a(2) = 3, a(3) = 6, …  (offset 1) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_zero_in_ternary.h"

static void ensure_capacity(size_t needed)
{
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    if (terms == NULL) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (terms == NULL) exit(1);
    }
    while (needed > terms_cap) {
        terms_cap *= 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (terms == NULL) exit(1);
    }
}

/* Static state to keep previously computed terms */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap = 0;
static uint64_t next_candidate = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined, harmless fallback */

    /* Ensure we have enough computed terms */
    while (terms_size < (size_t)n) {
        /* allocate or grow array on first use */
        if (terms == NULL) {
            terms_cap = 64;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            if (terms == NULL) exit(1);
            terms_size = 0;
        } else if (terms_size >= terms_cap) {
            terms_cap *= 2;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            if (terms == NULL) exit(1);
        }

        /* Find next number with a zero ternary digit */
        while (true) {
            seq_t cand = (seq_t)next_candidate;
            next_candidate++;                    /* advance for next time */
            if (has_zero_in_ternary(cand)) {
                terms[terms_size++] = cand;
                break;
            }
        }
    }

    return terms[(size_t)(n - 1)];
}
