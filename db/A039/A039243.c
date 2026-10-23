/*
 * A039243  Numbers whose base-12 representation has the same number of 3's
 *          and 8's.
 * https://oeis.org/A039243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A039243: numbers whose base‑12 representation
   has the same number of 3's and 8's.  The implementation
   lazily scans integers and caches all found terms. */
#include <stdlib.h>
#include <stdbool.h>

/* Cached terms and current state */
static seq_t *terms = NULL;    /* array of found terms */
static idx_t terms_len = 0;    /* number of cached terms */
static size_t terms_cap = 0;   /* allocated capacity of the array */
static seq_t candidate = 0;    /* next integer to test */

/* Append a found term to the cache, resizing if necessary */
static void append_term(seq_t val)
{
    if (terms_cap == 0) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_len == terms_cap) {
        size_t new_cap = terms_cap * 2;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Return the n‑th term (n ≥ 1) of sequence A039243 */
seq_t a(idx_t n)
{
    /* Generate terms until at least the n‑th is cached */
    while (terms_len < n) {
        seq_t x = candidate;   /* copy candidate for inspection */
        unsigned cnt3 = 0, cnt8 = 0;

        if (x == 0) {          /* base‑12 "0" has 0 3's and 0 8's */
            /* counts already equal */
        } else {
            while (x > 0) {
                seq_t d = x % (seq_t)12;
                if (d == 3) {
                    ++cnt3;
                } else if (d == 8) {
                    ++cnt8;
                }
                x = x / (seq_t)12;
            }
        }

        if (cnt3 == cnt8) {
            append_term(candidate);
        }
        candidate = candidate + (seq_t)1;
    }
    return terms[n - 1];   /* a(1) is terms[0] */
}
