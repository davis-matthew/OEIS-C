/*
 * A055568  Numbers not greater than the sum of digits of their squares.
 * https://oeis.org/A055568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers not greater than the sum of digits of their squares.
   a(1) = 0, a(2) = 1, a(3) = 2, …  as given by the OEIS entry. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

static seq_t *terms = NULL;      /* cached terms */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* allocated capacity */
static idx_t  last_checked = 0;  /* next integer to test */

/* Ensure at least `needed` slots are allocated. */
static void ensure_terms_capacity(size_t needed)
{
    if (needed > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 10;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Append a new term to the cache. */
static void append_term(seq_t val)
{
    ensure_terms_capacity(terms_len + 1);
    terms[terms_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* offset 1: n>=1 */

    /* Return cached term if available. */
    if (n <= terms_len) return terms[n - 1];

    /* Generate terms until n is reached. */
    for (idx_t x = last_checked; terms_len < n; ++x) {
        seq_t sq   = (seq_t)x * (seq_t)x;          /* x^2 */
        seq_t digs = digit_sum(sq);                /* sum of digits of x^2 */
        if ((seq_t)x <= digs) append_term((seq_t)x);
    }

    /* Update last_checked for next call. */
    last_checked = (idx_t)terms[terms_len - 1] + 1;

    return terms[n - 1];
}
