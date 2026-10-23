/*
 * A044509  Numbers n such that string 4,2 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation:  Generates the sequence A044509 by scanning successive
 * integers and caching discovered terms.  A term n is stored when
 *
 *   1. n contains the digit pattern "42" in its base‑6 representation
 *   2. n+1 does NOT contain that pattern.
 *
 * The cache allows arbitrary random access to a(n) for any n ≥ 1. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

/* Return true iff the base‑6 representation of x has the consecutive
 * digits 4 followed by 2 (from most significant to least). */
static bool has42(idx_t x)
{
    /* For numbers with a single digit no pattern is possible. */
    if (x < 6) return false;

    /* prev_digit holds the lower‑order digit of the pair being examined. */
    idx_t prev_digit = x % 6;
    x /= 6;
    while (x > 0) {
        idx_t d = x % 6;           /* next higher digit */
        x /= 6;
        if (prev_digit == 2 && d == 4) /* pattern 4,2 in original order */
            return true;
        prev_digit = d;
    }
    return false;
}

/* Cached data.  Static ensures persistence across calls. */
static seq_t *terms      = NULL;   /* array of stored terms          */
static idx_t terms_count  = 0;     /* number of terms currently cached */
static idx_t terms_cap    = 0;     /* allocated capacity in terms      */
static idx_t last_checked = 0;     /* last integer examined           */

/* Ensure the terms array has at least 'min' slots.  Uses seq_alloc
 * to allocate correctly for all builds (including arbitrary‑precision). */
static void ensure_terms_capacity(idx_t min)
{
    if (terms_cap >= min) return;

    idx_t new_cap = terms_cap ? terms_cap * 2 : 1;
    while (new_cap < min) new_cap *= 2;

    seq_t *new_terms = seq_alloc(new_cap);
    if (!new_terms) abort();

    for (idx_t i = 0; i < terms_count; ++i)
        new_terms[i] = terms[i];

    terms = new_terms;
    terms_cap = new_cap;
}

/* Extend the cache until it contains at least 'n' terms. */
static void expand_terms_to(idx_t n)
{
    if (terms_count >= n) return;

    ensure_terms_capacity(n);
    while (terms_count < n) {
        ++last_checked;                          /* examine next integer */
        if (has42(last_checked) && !has42(last_checked + 1))
            terms[terms_count++] = last_checked;  /* store term */
    }
}

/* Public function required by the build.  Returns the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but safe */
    expand_terms_to(n);
    return terms[n - 1];
}
