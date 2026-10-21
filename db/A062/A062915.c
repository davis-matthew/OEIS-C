/*
 * A062915  Non-palindromic number and its reversal are both multiples of 17.
 * https://oeis.org/A062915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer that is non‑palindromic and
 *            both the number and its digit reversal are multiples of 17
 * Range:     u8 none, u16 n <= 185, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_uint64.h"

/* Return the nth term of A062915.  idx_t is an unsigned integer type. */
seq_t a(idx_t n) {
    /* Static cache of already found terms. */
    static seq_t *terms = NULL;
    static size_t terms_count = 0;
    static size_t terms_capacity = 0;
    static uint64_t next_candidate = 1;

    /* Ensure we have enough space for the needed number of terms. */
    if (terms_capacity == 0) {
        terms_capacity = 16384;                     /* initial capacity   */
        terms = (seq_t *)malloc(terms_capacity * sizeof(seq_t));
        if (!terms) abort();
    } else if (terms_capacity <= terms_count) {
        size_t new_cap = terms_capacity * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_capacity = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_count < (size_t)n) {
        if ((next_candidate % 17ULL) == 0ULL) {
            uint64_t rev = reverse_uint64(next_candidate);
            if ((rev % 17ULL) == 0ULL && rev != next_candidate) {
                terms[terms_count++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
