/*
 * A199593  Numbers n such that 3n-2, 3n-1 and 3n are all composite.
 * https://oeis.org/A199593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n-th integer n such that 3n-2, 3n-1, and 3n
   are all composite.  The implementation searches sequentially and
   caches already found terms.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static idx_t *terms = NULL;          /* values of n that satisfy the condition */
static size_t terms_size = 0;        /* number of stored terms */
static size_t terms_cap  = 0;        /* allocated capacity of the array */
static idx_t  next_test  = 2;        /* next integer to test for inclusion */

/* ensure the array can hold at least `min` elements */
static void grow_terms(size_t min)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < min) new_cap <<= 1;

    idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* add a new term */
static void add_term(idx_t n)
{
    if (terms_size >= terms_cap) grow_terms(terms_size + 1);
    terms[terms_size++] = n;
}

/* Return the n‑th term of A199593 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for the offset 0 */

    while (terms_size < n) {
        /* test candidate n = next_test */
        idx_t i = next_test;
        seq_t m3 = (seq_t)3 * (seq_t)i;
        seq_t n2 = m3 - 2;
        seq_t n1 = m3 - 1;

        bool comp1 = n2 > 1 && !isPrimeGeneric(n2);
        bool comp2 = n1 > 1 && !isPrimeGeneric(n1);
        bool comp3 = m3 > 1 && !isPrimeGeneric(m3);

        if (comp1 && comp2 && comp3) {
            add_term(i);
        }
        next_test++;
    }
    return (seq_t)terms[n - 1];
}
