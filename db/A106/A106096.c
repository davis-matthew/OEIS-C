/*
 * A106096  Primes with maximal digit 6.
 * https://oeis.org/A106096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 501, u32 n <= 9686, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   has_max_digit6, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th prime whose maximal decimal digit is 6 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(uint64_t) */
#include "../utils/has_max_digit6.h"

static seq_t *terms = NULL;        /* collected terms */
static idx_t terms_count = 0;      /* # terms stored */
static idx_t terms_cap = 0;        /* array capacity */
static uint64_t next_candidate = 2;/* next integer to test */

/* ensure there is room for one more term */
static void grow_terms_array(void)
{
    idx_t new_cap = (terms_cap == 0 ? 16 : terms_cap * 2);
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();   /* allocation failed */
    terms = new_ptr;
    terms_cap = new_cap;
}

/* populate terms until we have at least n terms */
static void generate_terms(idx_t n)
{
    while (terms_count < n) {
        if (is_prime(next_candidate) && has_max_digit6(next_candidate)) {
            if (terms_count >= terms_cap) grow_terms_array();
            terms[terms_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 */
    generate_terms(n);
    return terms[n - 1];
}
