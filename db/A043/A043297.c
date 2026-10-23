/*
 * A043297  Primes p such that B(4*p) has denominator 30 where B(2n) are the
 *          Bernoulli numbers.
 * https://oeis.org/A043297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth prime p such that 2p+1 and 4p+1 are composite.
 *            The special prime 2 is also included
 * Range:     u8 n <= 24, u16 n <= 4962, u32 n <= 8839, u64 n <= 8839,
 *            u128 n <= 8839, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"


static seq_t *terms   = NULL;   /* stored terms  */
static size_t terms_len  = 0;    /* number of terms known */
static size_t terms_cap  = 0;    /* allocated capacity */
static idx_t  last_checked = 1;  /* last integer examined */

/* add a term to the cache, growing the array if required */
static void add_term(seq_t p)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = p;
}

/* The required sequence access function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 – return dummy */

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        /* find next candidate to test */
        idx_t cand = last_checked + 1U;
        if (cand > 2ULL && (cand & 1ULL) == 0ULL) {
            /* skip even numbers greater than 2 */
            cand++;
        }

        if (is_prime(cand)) {
            if (cand == 2ULL ||
                (!is_prime(2ULL * cand + 1ULL) && !is_prime(4ULL * cand + 1ULL))) {
                add_term((seq_t)cand);
            }
        }

        last_checked = cand;   /* remember the last examined integer */
    }

    return terms[n - 1];
}
