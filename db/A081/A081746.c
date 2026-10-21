/*
 * A081746  a(1)=1; for n>1, a(n)=a(n-1)-3 if n is already in the sequence,
 *          a(n)=a(n-1)+5 otherwise.
 * https://oeis.org/A081746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, hear
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) - 3 if n has appeared as a
 *            term of the sequence, otherwise a(n) = a(n-1) + 5
 * Range:     u8 n <= 115, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --- Static storage for the memoized sequence terms --- */
static seq_t *terms      = NULL;   /* terms[0] stores a(1) */
static size_t terms_cap  = 0;
static size_t terms_len  = 0;

/* Ensure the terms array has at least `min_cap` elements.  */
static void ensure_terms_capacity(size_t min_cap)
{
    if (terms_cap >= min_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = p;
    terms_cap = new_cap;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n = 0, return 0 as a guard */
        return 0;

    /* initialise on first call */
    if (terms_len == 0) {
        ensure_terms_capacity(16);
        terms[0] = (seq_t)1;   /* a(1) = 1 */
        terms_len = 1;
    }

    /* compute missing terms up to index n */
    while (terms_len < n) {
        idx_t      idx   = terms_len + 1;          /* current recurrence index */
        seq_t      prev  = terms[terms_len - 1];
        bool       in_seq = false;

        /* linear test: is `idx` already a term of the sequence? */
        for (idx_t i = 0; i < terms_len; ++i) {
            if (terms[i] == (seq_t)idx) {
                in_seq = true;
                break;
            }
        }

        seq_t val = in_seq ? prev - (seq_t)3 : prev + (seq_t)5;

        /* store the new term */
        ensure_terms_capacity(terms_len + 1);
        terms[terms_len] = val;
        ++terms_len;
    }

    return terms[n - 1];
}
