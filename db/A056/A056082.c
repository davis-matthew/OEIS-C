/*
 * A056082  Numbers k such that k^4 == 1 (mod 5^3).
 * https://oeis.org/A056082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer k such that k^4 ≡ 1 (mod 125)
 *            (i.e., k ≡ 1,57,68,124 (mod 125))
 * Range:     u8 n <= 9, u16 n <= 2097, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>

/* The sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* behaviour below offset is irrelevant */

    /* static state for memoization */
    static seq_t *terms = NULL;      /* stored terms */
    static size_t terms_len = 0;     /* number of stored terms */
    static size_t terms_cap = 0;     /* allocated capacity */
    static idx_t next_k = 1;         /* next integer to test */

    /* Extend the terms array up to index n */
    while (terms_len < n) {
        /* Find the next integer k that satisfies k ≡ 1,57,68,124 (mod 125) */
        idx_t k = next_k;
        while (1) {
            idx_t r = k % 125U;
            if (r == 1U || r == 57U || r == 68U || r == 124U)
                break;
            ++k;
        }

        /* Ensure capacity */
        if (terms_len == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 128;
            seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_terms) {
                /* In a production build you might handle allocation failure */
                abort();
            }
            terms = new_terms;
            terms_cap = new_cap;
        }

        terms[terms_len++] = (seq_t)k; /* store the term */
        next_k = k + 1;                /* next candidate starts after this k */
    }

    return terms[n - 1]; /* zero‑based array indexing */
}
