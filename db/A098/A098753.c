/*
 * A098753  a(n+1) = least positive integer not already used that begins with
 *          the last two digits of a(n).
 * https://oeis.org/A098753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   second_last_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 10; for n > 1, a(n) is the least positive integer not already used
   that begins with the last two digits of a(n‑1), and whose second‑to‑last digit is not 0. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/second_last_nonzero.h"

static seq_t *terms      = NULL;  /* stored sequence terms (also the used set) */
static idx_t  terms_len  = 0;
static idx_t  terms_cap  = 0;

/* Append a new term to the sequence array, expanding storage if needed */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap ? terms_cap * 2 : 1);
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* Check whether a value has already appeared in the sequence */
static bool used(seq_t val)
{
    for (idx_t i = 0; i < terms_len; ++i)
        if (terms[i] == val)
            return true;
    return false;
}

/* Return the least unused integer matching the required prefix */
static seq_t next_candidate(seq_t prefix)
{
    seq_t pow10k = 1;                    /* 10^k, starts with k = 0 */
    for (;;) {
        seq_t start = prefix * pow10k;
        seq_t end   = (prefix + 1) * pow10k - 1;
        /* Scan the block [start, end] in ascending order */
        for (seq_t cand = start; ; ++cand) {
            if (cand > end) break;        /* move to next block (k++) */
            if (second_last_nonzero(cand) && !used(cand))
                return cand;
        }
        /* Increase block length (k++) */
        pow10k *= 10;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1, but guard anyway */

    /* Ensure the sequence has been generated up to index n */
    if (terms_len == 0)                 /* initialise with the first term */
        append_term((seq_t)10);

    while (terms_len < n) {
        seq_t prev   = terms[terms_len - 1];
        seq_t prefix = prev % 100;       /* last two digits (tens digit is never 0) */
        seq_t cand   = next_candidate(prefix);
        append_term(cand);
    }
    return terms[n - 1];
}
