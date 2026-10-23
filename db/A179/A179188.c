/*
 * A179188  Numbers n such that phi(n) = phi(n+6), with Euler's totient
 *          function phi=A000010.
 * https://oeis.org/A179188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A179188: numbers n such that Phi(n) = Phi(n+6) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* seq_alloc_array(size_t) */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t*) */

/* ----- static storage ----- */
static seq_t *phi = NULL;           /* phi[0..phi_limit]    */
static idx_t phi_limit = 0;         /* current max index of phi   */

static seq_t *terms = NULL;         /* found sequence terms      */
static size_t terms_count = 0;      /* number of terms stored   */
static size_t terms_cap   = 0;      /* capacity of terms        */

static idx_t processed_up_to = 0;   /* highest n that has been scanned */

/* ----- helper: grow the terms array ----- */
static void grow_terms(size_t min_cap)
{
    if (min_cap <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = alloc_seq_array(new_cap);
    if (!tmp) return;                 /* OOM – silently abort */
    for (size_t i = 0; i < terms_count; ++i) tmp[i] = terms[i];
    if (terms) free_seq_array(terms);
    terms = tmp;
    terms_cap = new_cap;
}

/* ----- helper: append a new term ----- */
static void append_term(seq_t val)
{
    if (terms_count == terms_cap) grow_terms(terms_cap ? terms_cap * 2 : 16);
    terms[terms_count++] = val;
}

/* ----- recompute the totient table up to 'limit' (inclusive) ----- */
static void recompute_phi(idx_t limit)
{
    if (phi) free_seq_array(phi);
    phi = alloc_seq_array(limit + 1);
    if (!phi) return;                 /* OOM – silently abort */

    for (idx_t i = 0; i <= limit; ++i) phi[i] = (seq_t)i;

    for (idx_t p = 2; p <= limit; ++p) {
        if (phi[p] == (seq_t)p) {                  /* p is prime   */
            for (idx_t j = p; j <= limit; j += p) {
                seq_t t = phi[j] / (seq_t)p;
                phi[j] = phi[j] - t;
            }
        }
    }
    phi_limit = limit;
}

/* ----- public sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    while (terms_count < (size_t)n) {
        /* Determine a new search limit (doubling strategy) */
        idx_t new_limit;
        if (processed_up_to == 0) new_limit = 2000;
        else {
            new_limit = processed_up_to * 2;
            if (new_limit < processed_up_to + 2000) new_limit = processed_up_to + 2000;
        }

        /* Ensure phi table covers up to new_limit + 6 (needed for phi[i+6]) */
        if (new_limit + 6 > phi_limit) recompute_phi(new_limit + 6);

        /* Scan the new range for equal totients */
        for (idx_t i = processed_up_to + 1; i <= new_limit; ++i) {
            if (phi[i] == phi[i + 6]) append_term((seq_t)i);
        }

        processed_up_to = new_limit;
    }

    return terms[n - 1];
}
