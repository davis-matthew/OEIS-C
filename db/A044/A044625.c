/*
 * A044625  Numbers n such that string 7,1 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has71
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044625:
   Numbers n such that the base‑8 representation of n contains the
   substring "71", but the base‑8 representation of n+1 does not. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/has71.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t term_cap = 0;
    static size_t term_cnt = 0;
    static seq_t cur = 1;          /* next integer to test */

    /* Ensure capacity for n terms */
    if (n > term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 128;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_buf) abort();
        if (terms) {
            memcpy(new_buf, terms, term_cnt * sizeof(seq_t));
            free(terms);
        }
        terms = new_buf;
        term_cap = new_cap;
    }

    /* Generate terms until we have at least n */
    while (term_cnt < n) {
        /* Search for next qualifying integer */
        while (true) {
            if (!has71(cur)) { cur++; continue; }
            seq_t nxt = cur + 1;
            if (!has71(nxt)) break;
            cur++;
        }

        terms[term_cnt++] = cur;
        cur++;                    /* next search starts after this number */
    }

    return terms[n - 1];
}
