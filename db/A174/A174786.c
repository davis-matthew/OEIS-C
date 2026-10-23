/*
 * A174786  Numbers n congruent to 3 (mod 6) such that n+2 and n+8 are primes.
 * https://oeis.org/A174786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 695, u32 n <= 872, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A174786
 *  a(n) = numbers congruent to 3 (mod 6) with n+2 and n+8 both prime.
 *  The function generates terms on demand by iterating candidates and
 *  testing primality with is_prime_big().
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Static state to keep generated terms across calls. */
    static idx_t next_m = 0;          /* m such that k = 6*m+3 */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;

    if (n == 0) return (seq_t)0;  /* Undefined, but avoid crash. */

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < n) {
        seq_t k = (seq_t)(6 * next_m + 3);
        /* Check that k+2 and k+8 are primes. */
        if (is_prime_big(k + 2) && is_prime_big(k + 8)) {
            /* Grow storage if necessary. */
            if (terms_cnt == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                terms = tmp;  /* realloc never fails on supported builds. */
            }
            terms[terms_cnt++] = k;
        }
        next_m++;
    }

    return terms[n - 1];
}
