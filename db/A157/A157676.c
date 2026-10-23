/*
 * A157676  Numbers n such that n + (product of digits of n) is prime.
 * https://oeis.org/A157676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer m such that m + digit_product(m)
 *            is prime
 * Range:     u8 n <= 16, u16 n <= 4699, u32 n <= 4699, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is built on demand; every call returns the nth term without assuming increasing order. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *terms = NULL;     /* array of generated terms */
    static size_t terms_cap = 0;    /* allocated capacity */
    static size_t terms_len = 0;    /* number of generated terms */
    static idx_t candidate = 1;     /* next integer to test */

    /* generate terms until we reach the requested index */
    while (terms_len < n) {
        seq_t cand = (seq_t)candidate;
        seq_t prod = digit_product(cand);
        seq_t sum  = cand + prod;            /* m + product of digits */

        if (is_prime_big(sum)) {
            /* ensure capacity and store the term */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 4 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return 0;            /* abort on allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = cand;
        }
        candidate++;                          /* move to next integer to test */
    }

    /* nth term is stored at index n-1 */
    return terms[n - 1];
}
