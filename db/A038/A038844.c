/*
 * A038844  Numbers k for which 6*k+1 divides 2^k-1.
 * https://oeis.org/A038844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k satisfying 6*k+1 | 2^k - 1. The
 *            implementation scans k in increasing order, testing the
 *            congruence 2^k ≡ 1 (mod 6*k+1) by modular exponentiation
 * Range:     u8 n <= 3, u16 n <= 638, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   cmp_seq_t, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"
#include "../utils/cmp_seq_t.h"

static seq_t *terms = NULL;     /* stored terms */
static size_t terms_len = 0;    /* number of terms stored */
static size_t terms_cap = 0;    /* allocated capacity */
static idx_t current_max_k = 0; /* largest k already examined */

/* Ensure that the sequence contains at least n_target terms. */
static void ensure_terms(idx_t n_target)
{
    while (terms_len < n_target) {
        /* next candidate k */
        idx_t k = current_max_k + 1;
        current_max_k = k;

        /* m = 6*k+1 */
        seq_t m = (seq_t)k * 6 + 1;
        /* check 2^k ≡ 1 (mod m) */
        seq_t rem = mod_pow((seq_t)2, k, m);

        seq_t one = 1;
        if (cmp_seq_t(&rem, &one) == 0) {
            /* grow the array if needed */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *new_terms = seq_realloc(terms, terms_cap, new_cap);
                if (!new_terms) abort();          /* out of memory */
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;                 /* store the term */
        }
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    ensure_terms(n);              /* compute up to the required term */
    return terms[n - 1];          /* zero‑based array index */
}
