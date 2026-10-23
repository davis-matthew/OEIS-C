/*
 * A102251  Begin with 1, multiply each digit by 2.
 * https://oeis.org/A102251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102251:  "Begin with 1, multiply each digit by 2."
   a(0) = 1; for each term a(n), expand into 2*d for every decimal
   digit d of a(n).  Children are appended to the end of the sequence
   and processed in the same left‑to‑right order.  This is a breadth‑first
   expansion of the implicit tree defined by the rule. */

#include <stdlib.h>

/* Static state for lazy generation */
static seq_t *terms = NULL;     /* array holding the sequence */
static size_t terms_len = 0;    /* number of terms stored so far   */
static size_t terms_cap = 0;    /* allocated capacity               */
static idx_t  expanded = 0;     /* next index whose children to
                                  expand (queue head)              */

/* Ensure capacity for at least min_cap slots (C++ requires explicit cast). */
static void ensure_terms_cap(size_t min_cap)
{
    if (min_cap <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : min_cap;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!newp)
        abort(); /* out of memory */
    terms     = newp;
    terms_cap = new_cap;
}

/* Expand the term at index idx: enqueue its children (2*d for each
   decimal digit d of terms[idx]) in the order of the digits. */
static void expand_term(idx_t idx)
{
    seq_t cur = terms[idx];
    /* Extract digits in reverse order */
    unsigned char digits[256];
    size_t nd = 0;
    seq_t temp = cur;
    if (temp == (seq_t)0) {          /* only possible if a zero ever appears */
        digits[nd++] = 0;
    } else {
        while (temp != (seq_t)0) {
            digits[nd++] = (unsigned char)(temp % (seq_t)10);
            temp /= (seq_t)10;
        }
    }
    /* Append children in the forward digit order */
    for (size_t i = 0; i < nd; ++i) {
        seq_t child = (seq_t)digits[nd - 1 - i] * (seq_t)2;
        ensure_terms_cap(terms_len + 1);
        terms[terms_len++] = child;
    }
}

/* Public interface required by the problem statement */
seq_t a(idx_t n)
{
    /* Lazy construction: initialise first term on first call */
    if (terms == NULL) {
        ensure_terms_cap(1);
        terms[0] = (seq_t)1;
        terms_len = 1;
    }

    /* Generate terms until we have at least (n+1) entries. */
    while (terms_len <= (size_t)n) {
        expand_term(expanded);
        ++expanded;
    }

    return terms[n];
}
