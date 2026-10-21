/*
 * A053661  For n > 1: if n is present, 2n is not.
 * https://oeis.org/A053661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A053661.
   Sequence definition (in OEIS notation):
   a(1) = 1,
   for n > 1:  if a(i) has been chosen for some i < n,
               then 2 * a(i) is forbidden.
   The sequence is the lexicographically minimal sequence of positive
   integers satisfying the above property.  Equivalently,
   a(n) = mex{ a(i), 2*a(i) : 1 <= i < n }  (with a(1)=1).
*/

#include <stdlib.h>
#include <string.h>

/* Helper functions to grow dynamic arrays */
static void * grow_array(void *ptr, size_t new_cap, size_t elem_size) {
    void *newp = realloc(ptr, new_cap * elem_size);
    if (!newp) abort(); /* memory exhausted */
    return newp;
}

static void ensure_used_capacity(size_t needed) {
    static size_t used_cap = 0;
    static uint8_t *used = NULL;
    if (used_cap < needed) {
        size_t new_cap = used_cap ? (used_cap << 1) : 1024;
        while (new_cap < needed) new_cap <<= 1;
        used = (uint8_t *)grow_array(used, new_cap, sizeof(uint8_t));
        memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
        used_cap = new_cap;
    }
    /* make used array available to the main function via static vars */
    /* We'll keep a static pointer to the used array */
    /* Since we cannot create a truly static variable inside this function,
       we use a static local static variable outside the function scope. */
}

static void ensure_terms_capacity(size_t needed) {
    /* similar helpers for terms array */
    static size_t terms_cap = 0;
    static seq_t *terms = NULL;
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? (terms_cap << 1) : 16;
        while (new_cap < needed) new_cap <<= 1;
        terms = (seq_t *)grow_array(terms, new_cap, sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* Global static state for the sequence generation */
static seq_t *seq_terms = NULL;
static size_t seq_terms_cap = 0;
static size_t seq_terms_len = 0;

static uint8_t *used_terms = NULL;
static size_t used_terms_cap = 0;

static size_t next_candidate = 1;   /* smallest integer to start searching from */

static void init_sequence(void) {
    if (seq_terms == NULL) {
        seq_terms_cap = 16;
        seq_terms = (seq_t *)malloc(seq_terms_cap * sizeof(seq_t));
    }
    if (used_terms == NULL) {
        used_terms_cap = 1024;
        used_terms = (uint8_t *)calloc(used_terms_cap, sizeof(uint8_t));
    }
}

static void ensure_used_capacity_local(size_t needed) {
    if (used_terms_cap < needed) {
        size_t new_cap = used_terms_cap ? (used_terms_cap << 1) : 1024;
        while (new_cap < needed) new_cap <<= 1;
        used_terms = (uint8_t *)realloc(used_terms, new_cap * sizeof(uint8_t));
        memset(used_terms + used_terms_cap, 0, (new_cap - used_terms_cap) * sizeof(uint8_t));
        used_terms_cap = new_cap;
    }
}

static void ensure_terms_capacity_local(size_t needed) {
    if (seq_terms_cap < needed) {
        size_t new_cap = seq_terms_cap ? (seq_terms_cap << 1) : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
        seq_terms_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* sequence is defined for n >= 1 */

    init_sequence();

    while (seq_terms_len < (size_t)n) {
        /* find next unused positive integer */
        size_t cand = next_candidate;
        ensure_used_capacity_local(cand + 1);
        while (used_terms[cand]) {
            ++cand;
            ensure_used_capacity_local(cand + 1);
        }

        /* append new term */
        ensure_terms_capacity_local(seq_terms_len + 1);
        seq_terms[seq_terms_len] = (seq_t)cand;
        ++seq_terms_len;

        /* mark the term itself as used */
        used_terms[cand] = 1;

        /* exclude its double for n > 1 (i.e., not for the first term) */
        if (seq_terms_len > 1) {
            size_t dbl = cand << 1;   /* cand * 2 */
            ensure_used_capacity_local(dbl + 1);
            used_terms[dbl] = 1;
        }

        /* set candidate for next search; it will be incremented in the next loop if still used */
        next_candidate = cand;
    }

    return seq_terms[n - 1];
}
