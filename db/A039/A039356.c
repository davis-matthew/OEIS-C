/*
 * A039356  Numbers whose base-8 representation has the same nonzero number of
 *          6's and 7's.
 * https://oeis.org/A039356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th positive integer whose base‑8 representation
 *            contains * an equal non‑zero number of digits 6 and 7
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cached terms */
    static seq_t *terms = NULL;
    static size_t size  = 0;   /* number of terms cached */
    static size_t cap   = 0;   /* capacity of terms array */
    /* Next candidate to test */
    static seq_t next_val = 54; /* start just before first term (55) */

    /* Initialize cache on the first call */
    if (terms == NULL) {
        cap = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        /* In case of allocation failure, abort */
        if (terms == NULL) { abort(); }
        size = 0;
        next_val = 54; /* first candidate will be 55 */
    }

    /* Generate terms until we have at least n of them */
    while (size < (size_t)n) {
        ++next_val;                     /* next candidate integer */
        seq_t temp = next_val;
        int cnt6 = 0, cnt7 = 0;

        /* Count digits in base 8 */
        while (temp > 0) {
            int d = (int)(temp % 8);
            if (d == 6) ++cnt6;
            else if (d == 7) ++cnt7;
            temp /= 8;
        }

        /* Store if it satisfies the condition */
        if (cnt6 == cnt7 && cnt6 > 0) {
            if (size == cap) {          /* grow cache if necessary */
                cap *= 2;
                seq_t *new_terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (new_terms == NULL) { abort(); }
                terms = new_terms;
            }
            terms[size++] = next_val;
        }
    }

    return terms[n - 1];
}
