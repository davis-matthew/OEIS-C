/*
 * A224827  Primes p such that prime(floor(p/10) + (p mod 10)) = p.
 * https://oeis.org/A224827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) computed by searching primes p such that p is the k-th
 *            prime with k = floor(p/10)+p%10
 * Range:     u8 none, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_array = NULL;
static size_t prime_count = 0;
static const uint64_t PRIME_LIMIT = 5000000ULL; /* high enough for all terms */

static seq_t *terms = NULL;
static size_t terms_count = 0;
static size_t terms_cap = 0;
static size_t prime_idx = 0;

seq_t a(idx_t n)
{
    /* Ensure prime list is ready */
    if (!prime_array) {
        build_primes_upto(PRIME_LIMIT, &prime_array, &prime_count);
    }

    /* Build terms up to the requested index */
    size_t needed = (size_t)n;
    while (terms_count < needed) {
        if (prime_idx >= prime_count) {
            /* Ran out of primes – should not happen with a large limit */
            return (seq_t)0;
        }

        seq_t p = (seq_t)prime_array[prime_idx];
        seq_t k = p / ((seq_t)10) + (p % ((seq_t)10));

        if (k >= 1 && (size_t)k <= prime_count) {
            seq_t pk = (seq_t)prime_array[(size_t)k - 1];
            if (pk == p) {
                /* Append p to the sequence */
                if (terms_count == terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *new_terms = (seq_t *)realloc(terms,
                                                        new_cap * sizeof(seq_t));
                    if (!new_terms) {
                        /* Allocation failed; abort */
                        return (seq_t)0;
                    }
                    terms = new_terms;
                    terms_cap = new_cap;
                }
                terms[terms_count++] = p;
            }
        }
        ++prime_idx;
    }

    return terms[needed - 1];
}
