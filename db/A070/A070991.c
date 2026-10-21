/*
 * A070991  Numbers n such that the trajectory of n under the `3x+1' map
 *          reaches n - 1.
 * https://oeis.org/A070991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer m (m ≥ 2) such that the Collatz
 *            trajectory of m contains m−1. This is implemented by brute‑force
 *            generation: test each candidate m in increasing order, keep
 *            those that satisfy the property, and return the n‑th one
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   reaches_mminus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reaches_mminus1.h"

static int initialized = 0;
static seq_t *terms = NULL;
static idx_t term_count = 0;
static idx_t term_capacity = 0;
static uint64_t next_candidate = 2;  /* start testing from 2 */

seq_t a(idx_t n)
{
    /* lazy initialization of the dynamic array */
    if (!initialized) {
        term_capacity = 128;
        terms = (seq_t *)malloc(term_capacity * sizeof(seq_t));
        term_count = 0;
        next_candidate = 2;
        initialized = 1;
    }

    /* generate terms until we have at least n of them */
    while (term_count < n) {
        if (reaches_mminus1(next_candidate)) {
            /* grow array if necessary */
            if (term_count == term_capacity) {
                term_capacity *= 2;
                terms = (seq_t *)realloc(terms,
                                         term_capacity * sizeof(seq_t));
            }
            terms[term_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;   /* move to the next integer */
    }

    return terms[n - 1];
}
