/*
 * A074240  Numbers k such that k-th prime = sigma(sigma(k)) + 1.
 * https://oeis.org/A074240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the integers k satisfying  prime(k) = sigma(sigma(k)) + 1.
   The function returns the n-th such k (1‑based indexing). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions provided by the build. */
#include "../utils/sigma_of.h"
#include "../utils/get_prime.h"

/* Cache for the terms that have been found. */
static seq_t *term_cache = NULL;   /* array of found k values */
static size_t term_len   = 0;      /* number of terms currently cached */
static size_t term_cap   = 0;      /* allocated capacity of the array   */
static idx_t  searched_k = 0;      /* largest k examined so far        */

/* Append a new term to the cache, growing it if necessary. */
static void append_term(seq_t val)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? term_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure → terminate */
        term_cache = tmp;
    }
    term_cache[term_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms cached. */
    while (term_len < (size_t)n) {
        /* Examine the next candidate k. */
        searched_k++;

        /* Compute sigma(k). */
        seq_t sigma1 = sigma_of((idx_t)searched_k);

        /* Compute sigma(sigma(k)).  sigma_of expects an idx_t argument, so
           we safely cast sigma1 to idx_t (the values involved are well below
           the 64‑bit limit for the range of n considered). */
        seq_t sigma2 = sigma_of((idx_t)sigma1);

        /* Candidate value for the prime. */
        seq_t candidate = sigma2 + (seq_t)1;

        /* k‑th prime.  get_prime returns a uint32_t; promote it to seq_t. */
        seq_t prime_k = (seq_t)get_prime(searched_k);

        if (candidate == prime_k) {
            append_term(searched_k);
        }
    }

    return term_cache[n - 1]; /* zero‑based indexing */
}
