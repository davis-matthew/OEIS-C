/*
 * A153767  Primes p such that 8*p - 9 is also prime.
 * https://oeis.org/A153767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 346, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153767 – primes p such that 8*p‑9 is also prime.
   Computation is performed by a simple on‑the‑fly prime search
   (trial division for primes ≤ c).  The function returns the
   n‑th such prime with 1‑based indexing. */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Memoised sequence terms.  terms[i] holds a(i+1). */
static seq_t  *terms      = NULL;
static size_t terms_len   = 0;
static size_t terms_cap   = 0;

/* Current candidate integer to test for primality. */
static idx_t  next_candidate = 2;

/* Grow the terms array to hold at least need elements. */
static void grow_terms(size_t need)
{
    size_t new_cap = terms_cap;
    if (new_cap == 0) new_cap = 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure that at least n terms have been computed. */
static void ensure_terms(idx_t n)
{
    while (terms_len < (size_t)n) {
        /* Find the next prime candidate. */
        while (!is_prime_small(next_candidate)) ++next_candidate;

        /* Test the property 8*p-9 prime. */
        seq_t qp = ((seq_t)next_candidate) * (seq_t)8 - (seq_t)9;
        if (is_prime_small(qp)) {
            if (terms_len == terms_cap) grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */
    ensure_terms(n);
    return terms[n - 1];
}
