/*
 * A044073  Numbers k such that string 3,0 occurs in the base 4 representation
 *          of k but not of k-1.
 * https://oeis.org/A044073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k whose base‑4 representation contains the
   substring "30" while k‑1 does not.  The implementation performs a brute
   force search and memoises the discovered terms. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* true if x in base‑4 contains the adjacent digit pair 3,0 (3 followed by 0) */
static bool has30(seq_t x)
{
    if (x == (seq_t)0)
        return false;
    seq_t prev = (seq_t)4;                 /* sentinel: no previous digit yet   */
    while (x > (seq_t)0) {
        seq_t digit = x % (seq_t)4;        /* current least‑significant digit   */
        x /= (seq_t)4;                    /* shift right one base‑4 digit     */
        if (digit == (seq_t)3 && prev == (seq_t)0)
            return true;                  /* pattern 3 (more significant) , 0 (less) */
        prev = digit;                     /* store for next iteration           */
    }
    return false;
}

/* Memoised storage of the sequence values */
static seq_t *terms      = NULL;      /* array of discovered terms          */
static size_t terms_len  = 0;          /* number of stored terms             */
static size_t terms_cap  = 0;          /* current allocated capacity         */
static seq_t next_candidate = (seq_t)1;/* next integer to test              */

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    while (terms_len < n) {
        seq_t k = next_candidate;
        if (has30(k) && !has30(k - (seq_t)1)) {
            /* Grow array if needed */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap == 0 ? (size_t)8 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();         /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;
        }
        next_candidate += (seq_t)1;
    }
    return terms[n - (idx_t)1];
}
