/*
 * A192218  Numbers k such that the number of distinct prime divisors of k
 *          divides phi(k).
 * https://oeis.org/A192218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 236, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the number of distinct prime divisors of k divides φ(k). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/phi.h"

seq_t a(idx_t n) {
    /* static cache of terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    /* next integer to test */
    static idx_t next_k = 2;

    /* grow cache until it contains at least n terms */
    while (terms_len < n) {
        uint64_t k = (uint64_t)next_k;           /* value to test */
        int omega = count_distinct_primes(k);    /* ω(k) */
        seq_t phi_k = phi(next_k);               /* φ(k) */
        if (omega != 0 && (phi_k % (seq_t)omega == 0)) {
            /* ensure space for new term */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
