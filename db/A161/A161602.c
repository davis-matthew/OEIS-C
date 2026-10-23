/*
 * A161602  Positive integers k that are greater than the value of the
 *          reversal of k's binary representation.
 * https://oeis.org/A161602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = n-th positive integer k satisfying k > rev(k), where
 * rev(k) is the integer obtained by reversing k's binary representation.
 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *terms = NULL;
    static size_t term_count = 0;
    static size_t term_capacity = 0;

    /* Static variable to keep track of the next integer to test */
    static idx_t next_k = 1;

    /* If requested term already computed, return it */
    if (n <= term_count)
        return terms[n - 1];

    /* Ensure cache has enough capacity for new terms */
    if (term_capacity == 0) {
        term_capacity = 32;
        terms = (seq_t *)malloc(term_capacity * sizeof(seq_t));
    }

    /* Expand cache capacity as needed */
    while (term_count < n) {
        /* Compute reverse of next_k in binary */
        idx_t x = next_k;
        idx_t rev = 0;
        while (x > 0) {
            rev = (rev << 1) | (x & 1);
            x >>= 1;
        }

        /* Test condition */
        if ((seq_t)next_k > (seq_t)rev) {
            /* Add to cache */
            if (term_count >= term_capacity) {
                term_capacity *= 2;
                terms = (seq_t *)realloc(terms, term_capacity * sizeof(seq_t));
            }
            terms[term_count++] = (seq_t)next_k;
        }

        /* Next integer to test */
        ++next_k;
    }

    return terms[n - 1];
}
