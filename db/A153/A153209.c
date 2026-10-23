/*
 * A153209  Primes of the form 2*p+1 where p is prime and p+1 is squarefree.
 * https://oeis.org/A153209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 2·p+1 where p is prime and p+1
 *            is squarefree
 * Range:     u8 n <= 5, u16 n <= 132, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

/* Cache of sequence terms */
static seq_t  *terms      = NULL;
static size_t  terms_cap  = 0;
static size_t  term_count = 0;
static uint64_t next_candidate = 2;   /* next integer to try as p */

/* Grow the cache to new_cap elements */
static void grow_terms(size_t new_cap)
{
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms && new_cap > 0) abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* Append a new term to the cache, growing if necessary */
static void append_term(seq_t v)
{
    if (term_count >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        grow_terms(new_cap);
    }
    terms[term_count++] = v;
}

/* Return the n-th term of A153209 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset < 1, but safe */

    /* Expand cache to contain at least n terms */
    while (term_count < n) {
        /* Find next p satisfying the conditions */
        for (;;) {
            if (is_prime((idx_t)next_candidate)) {
                uint64_t p  = next_candidate;
                uint64_t pp1 = p + 1;
                if (is_squarefree((seq_t)pp1)) {
                    uint64_t q = 2 * p + 1;
                    if (is_prime((idx_t)q)) {
                        seq_t val = (seq_t)q;
                        append_term(val);
                        ++next_candidate;
                        break;
                    }
                }
            }
            ++next_candidate;
        }
    }
    return terms[n - 1];
}
