/*
 * A228102  Numbers n such that sum of all primes <=n is prime.
 * https://oeis.org/A228102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 105, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228102: numbers n such that the sum of all primes ≤ n is prime.
   The function caches the sequence, so calls in any order are efficient. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cached terms of the sequence */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;

    /* Current state of the search: last integer examined
       and cumulative sum of primes up to it. */
    static idx_t current_n = 1;   /* last processed integer */
    static seq_t sum = 0;          /* sum of all primes ≤ current_n */

    /* Extend the cache until we have at least n terms */
    while (terms_len < n) {
        /* Move to the next integer */
        current_n++;

        /* If current_n is prime, add it to the running sum */
        if (is_prime(current_n))
            sum = sum + (seq_t)current_n;

        /* If the cumulative sum is prime, current_n is a term */
        if (is_prime_big(sum)) {
            /* Grow the cache if necessary */
            if (terms_len + 1 > terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* memory allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)current_n;
        }
    }

    return terms[n - 1];
}
