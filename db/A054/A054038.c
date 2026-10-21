/*
 * A054038  Numbers k such that k^2 contains every digit at least once.
 * https://oeis.org/A054038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: numbers k such that k^2 contains every decimal digit 0–9 at
    least once. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_digits.h"

/* Static cache of found terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Largest k that has already been tested. */
static idx_t last_k = 0;

/* Ensure the term cache has at least min_cap capacity. */
static void ensure_terms_cap(size_t min_cap)
{
    if (min_cap <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : min_cap;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    terms = new_ptr;
    terms_cap = new_cap;
}

/* Add a new term to the cache. */
static void add_term(seq_t val)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety check, offset is 1 */

    /* Produce terms until we have at least n items. */
    while (terms_len < (size_t)n)
    {
        ++last_k;
        unsigned __int128 sq = (unsigned __int128)last_k * last_k;
        if (count_distinct_digits(sq) == 10)   /* 10 distinct digits required */
            add_term(last_k);
    }
    return terms[n - 1];
}
