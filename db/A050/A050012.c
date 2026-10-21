/*
 * A050012  a(n) = floor(a(n-1)/2) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 7*a(n-1).
 * https://oeis.org/A050012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 312, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050012: a(n) = floor(a(n-1)/2) if this is positive and not yet in the
 * sequence, otherwise a(n) = 7*a(n-1).  The sequence starts with a(1) = 1. */
/* The function builds the sequence on demand and memoises all previously
 * generated terms for quick duplicate detection.  It uses only plain
 * C11 facilities and works with arbitrary‑precision seq_t types supplied
 * by the harness (casting the result of malloc/realloc is required). */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static storage for memoised terms. */
static seq_t *terms = NULL;
static size_t term_len = 0;
static size_t term_cap = 0;

/* Ensure that the array has at least 'needed' capacity.  The array is
 * grown by doubling which gives amortised O(1) growth. */
static void ensure_capacity(size_t needed)
{
    if (needed <= term_cap)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;        /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();              /* fatal error on allocation failure */
    }
    terms = tmp;
    term_cap = new_cap;
}

/* Return true if value 'v' is already in the sequence, false otherwise. */
static bool seen_in_terms(seq_t v)
{
    for (size_t i = 0; i < term_len; ++i) {
        if (terms[i] == v)
            return true;
    }
    return false;
}

/* Compute a(n) as defined.  The function is re‑entrant and memoises all
 * intermediates so successive calls are efficient. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for n=0, return 0 as placeholder */

    size_t target = (size_t)n;           /* n is guaranteed to fit in size_t */

    ensure_capacity(target);

    /* Initialise the sequence with a(1)=1 if it hasn't been yet. */
    if (term_len == 0) {
        terms[0] = (seq_t)1;
        term_len = 1;
    }

    while (term_len < target) {
        seq_t prev = terms[term_len - 1];
        seq_t cand = prev / (seq_t)2;    /* floor division */

        if (cand > (seq_t)0 && !seen_in_terms(cand))
            terms[term_len++] = cand;
        else
            terms[term_len++] = prev * (seq_t)7;
    }

    return terms[target - 1];
}
