/*
 * A044554  Numbers n such that string 5,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_sub50
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A044554 by brute‑force search:
   returns the n‑th integer whose base‑7 representation
   contains the substring “50” while the base‑7 representation
   of n + 1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_sub50.h"

/* Cache to speed up successive calls. */
static idx_t    terms_count = 0;        /* number of terms stored   */
static size_t   terms_alloc = 0;        /* allocated capacity       */
static seq_t  *terms       = NULL;      /* array of cached terms    */
static seq_t    last_seen   = 0;        /* integer last examined    */

/* Append a new term to the cache, reallocating if necessary. */
static void append_term(seq_t v)
{
    if (terms_count >= terms_alloc) {
        size_t new_alloc = terms_alloc ? terms_alloc * 2 : 64;
        seq_t *new_terms =
            (seq_t *)realloc(terms, new_alloc * sizeof(seq_t));
        /* Assume allocation succeeds – abort otherwise. */
        terms = new_terms;
        terms_alloc = new_alloc;
    }
    terms[terms_count++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined at offset 0  */
    /* Extend the cache until we have n terms. */
    while (terms_count < n) {
        if (has_sub50(last_seen) && !has_sub50(last_seen + 1)) {
            append_term(last_seen);
        }
        last_seen++;
    }
    return terms[n - 1];
}
