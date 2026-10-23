/*
 * A049233  Primes p such that p + 2 is squarefree.
 * https://oeis.org/A049233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 4911, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, squarefree_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049233: primes p such that p + 2 is square‑free.
   The function generates terms on demand and memoises them.
   offset = 1, therefore a(1) is 3.                                                        */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/squarefree_u64.h"

/* ------------------------------------------------------------------ */
/* Helper: test whether n is squarefree (no prime divides n twice).  */
/* Works for all 64‑bit values; the test is simple and fast enough   */
/* for the sizes needed (≤ 10 000th term).                           */

/* ------------------------------------------------------------------ */
/* Memoised storage of the sequence.                                   */
/* ------------------------------------------------------------------ */
static seq_t *terms = NULL;       /* array of computed terms          */
static size_t terms_cap = 0;      /* allocated capacity of 'terms'    */
static size_t terms_size = 0;     /* number of terms currently stored */
static uint64_t cand = 3;         /* next odd candidate to test       */

/* ------------------------------------------------------------------ */
/* Ensure that terms[0 .. n‑2] are filled (n ≥ 1).                    */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t n)
{
    if (n <= terms_size)
        return;                     /* already computed */

    /* initial allocation if necessary */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms)
            abort();                /* allocation failure */
    }

    /* extend storage as needed */
    while (terms_size < (size_t)n) {
        if (is_prime(cand)) {
            uint64_t plus2 = cand + 2ull;
            if (squarefree_u64(plus2)) {
                if (terms_size == terms_cap) {
                    size_t new_cap = terms_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_size++] = (seq_t)cand;
            }
        }
        cand += 2;  /* skip even numbers */
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for n < 1 */
    ensure_terms(n);
    return terms[n - 1];              /* offset 1 */
}
