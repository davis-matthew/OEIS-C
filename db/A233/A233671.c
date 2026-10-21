/*
 * A233671  Numbers k such that prime(k)^2 < prime(k-1)*prime(k+1).
 * https://oeis.org/A233671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that prime(k)^2 <
 *            prime(k-1)*prime(k+1)
 * Range:     u8 n <= 25, u16 n <= 3467, u32 n <= 3467, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    static bool initialized = false;
    static idx_t current_k = 2;
    static seq_t pk_minus = 0, pk = 0, pk_next = 0;
    static idx_t *terms = NULL;
    static size_t terms_cap = 0, terms_len = 0;

    if (!initialized) {
        /* Initialize prime values for k = 1,2,3 */
        pk_minus = (seq_t)get_prime(1);  /* p_1 */
        pk       = (seq_t)get_prime(2);  /* p_2 */
        pk_next  = (seq_t)get_prime(3);  /* p_3 */
        current_k = 2;                   /* k starts from 2 */
        initialized = true;
    }

    /* Ensure we have at least 'n' terms */
    while (terms_len < (size_t)n) {
        /* Check inequality: prime(k)^2 < prime(k-1)*prime(k+1) */
        if (pk * pk < pk_minus * pk_next) {
            /* Grow the terms array if necessary */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
                if (!tmp) abort();  /* Memory allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = current_k;
        }
        /* Advance to the next k */
        current_k++;
        pk_minus = pk;
        pk = pk_next;
        pk_next = (seq_t)next_prime((uint64_t)pk);  /* first prime > pk */
    }

    /* Return the requested term as seq_t */
    return (seq_t)terms[(size_t)(n - 1)];
}
