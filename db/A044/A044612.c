/*
 * A044612  Numbers n such that string 5,4 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has54
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044612: numbers n such that base8(n) contains the substring "54"
   and base8(n+1) does not contain "54". This implementation
   generates terms on demand by naïve scanning from the last
   tested integer up to the required index. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has54.h"

/* dynamic array of found terms */
static seq_t *terms = NULL;        /* array of terms */
static size_t terms_size = 0;      /* number of stored terms */
static size_t terms_cap  = 0;      /* allocated capacity */

/* last numeric value that has been tested (next to check is last_tested+1) */
static seq_t last_tested = 0;

/* append a new term to the array */
static void append_term(seq_t val) {
    if (terms_size == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 8;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_size++] = val;
}

/* build terms until we have at least n terms */
static void ensure_terms(idx_t n) {
    while ((idx_t)terms_size < n) {
        seq_t curr = last_tested + (seq_t)1;
        if (has54(curr) && !has54(curr + (seq_t)1)) {
            append_term(curr);
        }
        last_tested = curr;
    }
}

seq_t a(idx_t n) {
    if (n == 0) { /* undefined for n < 1, but return 0 */
        return (seq_t)0;
    }
    ensure_terms(n);
    return terms[n - 1];
}
