/*
 * A182078  Odd numbers n such that the reduced Collatz map n -> (3n+1)/2^k
 *          gives a trajectory of decreasing odd numbers.
 * https://oeis.org/A182078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182078: odd numbers whose reduced Collatz trajectory strictly decreases.
   Computed by testing successive odd numbers. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/odd_part.h"

static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static seq_t next_candidate = 3; /* first odd > 1 */

/* test whether an odd number n has a strictly decreasing reduced Collatz trajectory */
static bool satisfies(seq_t n) {
    if (n == (seq_t)1) return false; /* 1 never decreases */
    seq_t prev = n;
    while (true) {
        seq_t next = odd_part(prev * (seq_t)3 + (seq_t)1);
        if (next >= prev) return false;               /* failed: not decreasing */
        if (next == (seq_t)1) return true;            /* succeeded reaching 1 */
        prev = next;
    }
}

seq_t a(idx_t n) {
    /* generate terms until we have enough to answer */
    while (terms_len < n) {
        if (satisfies(next_candidate)) {
            if (terms_len == terms_cap) {
                size_t newcap = terms_cap == 0 ? 16 : terms_cap * 2;
                terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                terms_cap = newcap;
            }
            terms[terms_len++] = next_candidate;
        }
        next_candidate += (seq_t)2; /* next odd number */
    }
    return terms[n - 1];
}
