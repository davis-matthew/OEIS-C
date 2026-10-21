/*
 * A053030  Numbers with 2 zeros in Fibonacci numbers mod m.
 * https://oeis.org/A053030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that the Fibonacci
 *            sequence modulo m contains exactly two zeros in one Pisano
 *            period
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_two_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_two_zeros.h"

/* The sequence cache.  terms[i] stores a(i+1). */
static seq_t  *terms = NULL;
static size_t  terms_cap = 0;
static idx_t   terms_filled = 0;
static idx_t   next_candidate = 3;   /* next integer to test for inclusion */

/* Ensure that a(n) can be answered for all 1 <= n <= target.            */
static void ensure_terms(idx_t target)
{
    while (terms_filled < target) {
        /* test current candidate */
        if (has_two_zeros((uint64_t)next_candidate)) {
            if (terms_filled == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort the program safely. */
                    abort();
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_filled] = (seq_t)next_candidate;
            ++terms_filled;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for n < 1; return 0 as a safe default. */
        return (seq_t)0;
    }
    ensure_terms(n);
    return terms[n - 1];
}
