/*
 * A068567  Numbers k such that gpf(k) < k/gpf(k) and gpf(k/gpf(k)) < gpf(k),
 *          where gpf(n) is the greatest prime factor of n, A006530.
 * https://oeis.org/A068567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that gpf(k) < k/gpf(k) and gpf(k/gpf(k)) < gpf(k). */
/* Evaluates the n‑th term (offset 1) of OEIS A068567. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gpf_uint64.h"

/* Dynamic array holding the already‑computed terms of the sequence. */
static seq_t *terms = NULL;          /* array of computed terms        */
static idx_t  terms_alloc = 0;       /* present capacity               */
static idx_t  terms_size  = 0;       /* number of stored terms          */

/* Next candidate integer to examine.  Starts at 2 because k must be >1. */
static idx_t next_k = 2;

/* Ensure that |terms| has room for at least 'need' elements. */
static void ensure_terms_capacity(idx_t need)
{
    if (need <= terms_alloc) return;
    idx_t new_alloc = terms_alloc ? terms_alloc * 2 : 64;
    if (new_alloc < need) new_alloc = need;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_alloc * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE);   /* abort if memory allocation fails */
    terms = new_ptr;
    terms_alloc = new_alloc;
}

/* Return the n-th term (offset 1) of the sequence. */
seq_t a(idx_t n)
{
    while (terms_size < n) {            /* generate until we have 'n' terms */
        uint64_t k = next_k++;          /* current integer to test (as uint64_t) */
        uint64_t p = gpf_uint64(k);     /* greatest prime factor of k */

        /* Condition 1: p^2 < k   (equivalently  p < k/p) */
        if ((seq_t)p * (seq_t)p >= (seq_t)k)
            continue;

        /* m = k / p   (integer division, guaranteed) */
        seq_t m = (seq_t)k / (seq_t)p;
        /* Condition 2: m > p   (same as p < m)   */
        if (m <= (seq_t)p)
            continue;

        /* Condition 3: gpf(m) < p  */
        uint64_t q = gpf_uint64((uint64_t)m);
        if (q >= p)
            continue;

        /* All conditions satisfied – store the term. */
        ensure_terms_capacity(terms_size + 1);
        terms[terms_size++] = (seq_t)k;
    }
    /* Zero‑based indexing into the array. */
    return terms[n - 1];
}
