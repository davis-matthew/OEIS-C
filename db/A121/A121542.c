/*
 * A121542  Increasing sequence: "if n appears a*n+b does not", case a(1)=5,
 *          a=2, b=1.
 * https://oeis.org/A121542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the increasing sequence of integers starting at 5
   constructed by: whenever a term x appears, the number 2*x + 1 is
   forbidden forever. The function generates terms lazily, memoises
   them, and returns the n‑th term (1‑based).  It works for any
   unsigned integer type seq_t (basic or arbitrary‑precision) and
   standard 64‑bit idx_t. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1u) return 0;

    /* Memoised terms array */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap  = 0;

    /* Boolean array marking forbidden numbers (2*k+1 for k in terms). */
    static unsigned char *excluded = NULL;
    static size_t excl_cap = 0;

    /* Ensure terms array can hold n elements. */
    if (terms_cap < n) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 4;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }

    /* Macro to grow the excluded array so that index (need-1) is valid. */
    #define ENSURE_EXCL(need) \
        do { \
            if ((size_t)(need) > excl_cap) { \
                size_t new_cap = excl_cap ? excl_cap * 2 : 16; \
                while (new_cap < (size_t)(need)) new_cap *= 2; \
                unsigned char *tmp = (unsigned char *)realloc(excluded, new_cap); \
                if (!tmp) abort(); \
                if (new_cap > excl_cap) { \
                    size_t added = new_cap - excl_cap; \
                    memset(tmp + excl_cap, 0, added); \
                } \
                excluded = tmp; \
                excl_cap = new_cap; \
            } \
        } while (0)

    /* Start searching from the next integer after the last term,
       or from 5 if we have no terms yet. */
    seq_t candidate = (terms_size > 0) ? terms[terms_size - 1] + (seq_t)1 : (seq_t)5;

    /* Generate terms until we reach the desired count. */
    while (terms_size < n) {
        /* Skip any forbidden numbers. */
        while ( (size_t)candidate < excl_cap && excluded[(size_t)candidate] ) {
            candidate += (seq_t)1;
        }

        /* Record the candidate as the next term. */
        terms[terms_size++] = candidate;

        /* Mark its forbidden counterpart 2*candidate + 1. */
        seq_t temp = candidate * (seq_t)2 + (seq_t)1;
        idx_t excl_idx = (idx_t)temp;
        ENSURE_EXCL((size_t)excl_idx + 1);
        excluded[excl_idx] = 1;

        /* Advance candidate for the next search. */
        candidate += (seq_t)1;
    }

    #undef ENSURE_EXCL

    return terms[n - 1];
}
