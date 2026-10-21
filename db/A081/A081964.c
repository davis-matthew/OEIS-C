/*
 * A081964  In the following triangle the n-th row contains n numbers
 *          relatively prime to n and not occurring in a previous row. a(1) =
 *          1. Sequence contains the triangle by rows.
 * https://oeis.org/A081964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th element of the triangle where row r contains the
 *            r numbers coprime to r and not present in any previous row.
 *            Computed by generating rows incrementally
 * Range:     u8 n <= 235, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static state to allow incremental construction and memoisation */
    static seq_t *terms   = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    static uint8_t *used    = NULL;
    static size_t used_cap  = 0;
    static idx_t   current_row = 0;   /* rows generated so far */

    /* Return already computed value if possible */
    if (terms_len >= n)
        return terms[n - 1];

    /* Generate rows until we have at least n terms */
    while (terms_len < n) {
        current_row++;           /* new row number r */
        idx_t r = current_row;
        idx_t selected = 0;      /* how many numbers chosen in this row */
        idx_t cand = 1;          /* candidate integer to test */

        /* Search for r distinct numbers coprime to r and not used before */
        while (selected < r) {
            /* Ensure the used array covers the candidate */
            if ((size_t)cand >= used_cap) {
                size_t new_cap = used_cap == 0 ? 256 : used_cap * 2;
                while (new_cap <= (size_t)cand) new_cap <<= 1;
                uint8_t *new_used = (uint8_t *)malloc(new_cap);
                if (!new_used) exit(1);
                for (size_t i = 0; i < used_cap; ++i) new_used[i] = used[i];
                for (size_t i = used_cap; i < new_cap; ++i) new_used[i] = 0;
                free(used);
                used = new_used;
                used_cap = new_cap;
            }

            if (!used[(size_t)cand] && gcd((seq_t)cand, (seq_t)r) == 1) {
                /* Mark as used and store in terms */
                used[(size_t)cand] = 1;

                if (terms_len >= terms_cap) {
                    size_t new_cap = terms_cap == 0 ? 256 : terms_cap * 2;
                    seq_t *new_terms = (seq_t *)malloc(new_cap * sizeof(seq_t));
                    if (!new_terms) exit(1);
                    if (terms) {
                        memcpy(new_terms, terms, terms_len * sizeof(seq_t));
                        free(terms);
                    }
                    terms = new_terms;
                    terms_cap = new_cap;
                }

                terms[terms_len++] = (seq_t)cand;
                ++selected;
            }
            ++cand;
        }
    }

    return terms[n - 1];
}
