/*
 * A076746  List giving pairs of primes of the form 10k+3 and 10k+7.
 * https://oeis.org/A076746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 142, u32 n <= 142, u64 n <= 142,
 *            u128 n <= 142, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076746: primes p with p ≡ 3 or 7 (mod 10) such that p±4 is also prime.
   The sequence is the sorted union of both numbers in every admissible pair.
   For k = 0,1,2,… we test p₁ = 10k + 3 and p₂ = p₁ + 4.
   If both p₁ and p₂ are prime, we emit p₁ followed by p₂.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

static seq_t   *terms   = NULL;   /* dynamically growing array of results   */
static size_t   terms_len = 0;    /* number of terms currently stored      */
static size_t   terms_cap = 0;    /* current array capacity                */

static idx_t    next_k    = 0;    /* k value to be examined next           */

/* Grow the terms array if necessary.  */
static void grow_terms(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();             /* unrecoverable memory error            */
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure that at least n terms of the sequence have been generated. */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;            /* offset is 1; a(0) is undefined      */
    if (terms == NULL) {           /* first call: allocate initial memory  */
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();       /* unrecoverable                            */
    }
    while (terms_len < (size_t)n) {
        /* compute primes for current k */
        idx_t p1 = 10 * next_k + 3;   /* p1 = 10k + 3 */
        idx_t p2 = p1 + 4;            /* p2 = 10k + 7 */

        /* Test the pair; only when both primes do we store them. */
        if (is_prime(p1) && is_prime(p2)) {
            grow_terms(terms_len + 2); /* need space for two new terms  */
            terms[terms_len++] = (seq_t)p1;
            terms[terms_len++] = (seq_t)p2;
        }
        next_k++;                    /* advance to next k                      */
    }
}

/* External linkage required by the build.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1 – return 0 as default    */
    ensure_terms(n);
    return terms[n - 1];
}
