/*
 * A167466  Primes p such that (p reversed)+8 is also a prime.
 * https://oeis.org/A167466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p such that reverse_decimal(p)+8 is also
 *            prime
 * Range:     u8 n <= 7, u16 n <= 170, u32 n <= 170, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

static seq_t *terms = NULL;          /* array of found terms   */
static size_t terms_size = 0;        /* number of stored terms */
static size_t terms_cap  = 0;        /* current allocation size */
static idx_t   next_candidate = 2;   /* next integer to test   */

/* Grow the terms array when more space is required. */
static void grow_terms(void) {
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory            */
    terms = tmp;
    terms_cap = new_cap;
}

/* Return the nth term of the sequence (n ≥ 1).  Calls may be out of order. */
seq_t a(idx_t n) {
    /* Ensure that at least n terms have been computed. */
    while (terms_size < (size_t)n) {
        /* Skip even numbers > 2, they cannot be prime. */
        if (next_candidate > 2 && ((next_candidate & 1ULL) == 0)) {
            next_candidate++;
            continue;
        }

        seq_t p = (seq_t)next_candidate;
        if (is_prime_big(p)) {
            seq_t revp = reverse_decimal(p);
            seq_t revplus8 = revp + (seq_t)8;   /* same type as revp   */
            if (is_prime_big(revplus8)) {
                if (terms_size == terms_cap) grow_terms();
                terms[terms_size++] = p;
            }
        }
        next_candidate++;
    }
    return terms[(size_t)n - 1];
}
