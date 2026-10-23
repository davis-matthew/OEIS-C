/*
 * A161817  Positions n such that A010060(n) = A010060(n+5).
 * https://oeis.org/A161817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer k (k ≥ 0) for which popcount_2(k) mod 2
 *            = popcount_2(k + 5) mod 2 (i.e. the parity of the binary weight
 *            of k equals that of k+5)
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *terms = NULL;        /* dynamic array of found terms   */
static size_t terms_len = 0;       /* number of terms stored         */
static size_t terms_cap = 0;       /* allocated capacity             */
static idx_t next_candidate = 0;   /* next integer to test           */

/* Compute the parity (0 for even, 1 for odd) of the number
   of 1‑bits in the binary representation of x. */
static inline int parity_popcount(idx_t x)
{
    int p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;               /* clear lowest set bit */
    }
    return p;
}

/* Append a new term to the dynamic array, reallocating
   if necessary. */
static void add_term(idx_t v)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 8 : terms_cap * 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = (seq_t)v;
}

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms stored. */
    while (terms_len < n) {
        if (parity_popcount(next_candidate) ==
            parity_popcount(next_candidate + 5))
            add_term(next_candidate);
        next_candidate++;
    }
    return terms[n - 1];
}
