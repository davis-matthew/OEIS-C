/*
 * A015875  Numbers k such that phi(k + 12) | sigma(k) + 12.
 * https://oeis.org/A015875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 758, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015875: numbers k such that φ(k + 12) divides σ(k) + 12.
   The implementation searches incrementally for qualifying k,
   caching found terms so that any n≥1 can be answered in O(1) after
   the necessary precomputation.  Φ and σ are obtained via the
   utility functions `calc_phi` and `sigma_of`, which work with the
   build‑specific `seq_t` type. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/calc_phi.h"   /* seq_t calc_phi(seq_t m); */
#include "../utils/sigma_of.h"  /* seq_t sigma_of(idx_t n); */

static seq_t *terms   = NULL;   /* cached sequence values */
static size_t terms_cap = 0;     /* allocated capacity */
static size_t terms_len = 0;     /* number of stored terms */
static uint64_t last_k   = 0;    /* last k checked */

/* Append a new term to the cache, expanding storage if necessary. */
static void append_term(seq_t k)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = k;
}

/* Generate terms until the cache contains at least n elements. */
static void grow_terms(idx_t n)
{
    while (terms_len < (size_t)n) {
        ++last_k;                     /* test k = last_k + 1 */
        seq_t phi  = calc_phi((seq_t)(last_k + 12));      /* φ(k+12) */
        seq_t sigma = sigma_of((idx_t)last_k);           /* σ(k)  */
        sigma += (seq_t)12;                                 /* σ(k)+12 */
        if (sigma % phi == 0)                               /* divisibility */
            append_term((seq_t)last_k);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0; return 0 */
    grow_terms(n);                /* ensure cache holds the desired term */
    return terms[n - 1];           /* seq_t is expensive but exact */
}
