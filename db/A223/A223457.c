/*
 * A223457  Composite numbers whose number of proper divisors has a number of
 *          proper divisors which has a prime number of proper divisors.
 * https://oeis.org/A223457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th composite integer such that * let d1 = tau(n)-1,
 *            d2 = tau(d1)-1, d3 = tau(d2)-1; * then d3 is prime. The sequence
 *            is obtained by searching * composite numbers and testing this
 *            divisor‑chain condition
 * Range:     u8 none, u16 n <= 3, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   divisor_count_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/divisor_count_u64.h"

/* Static storage for memoized terms */
static seq_t *terms = NULL;   /* array of found terms   */
static size_t terms_len = 0;  /* number of stored terms */
static size_t terms_cap = 0;  /* current capacity       */
static uint64_t last_checked = 3;   /* candidate start at 4 */

/* Return the n‑th term of A223457 (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined, but harmless            */

    while (terms_len < n) {
        last_checked++;                /* new candidate                         */
        uint64_t cand = last_checked;

        /* Skip primes – only composites are allowed. */
        if (is_prime((idx_t)cand))
            continue;

        /* First level: proper divisor count of cand. */
        seq_t tau_cand = divisor_count_u64(cand);
        uint64_t d1 = (uint64_t)tau_cand - 1;
        if (d1 <= 1)            /* composite guarantees d1 ≥ 2, but guard anyway */
            continue;

        /* Second level: proper divisor count of d1. */
        seq_t tau_d1 = divisor_count_u64(d1);
        uint64_t d2 = (uint64_t)tau_d1 - 1;
        if (d2 <= 1)
            continue;

        /* Third level: proper divisor count of d2. */
        seq_t tau_d2 = divisor_count_u64(d2);
        uint64_t d3 = (uint64_t)tau_d2 - 1;

        /* d3 must be a prime number. */
        if (d3 >= 2 && is_prime((idx_t)d3)) {
            /* Grow the terms array if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *new_terms =
                    (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms)     /* allocation failure – abort */
                    return (seq_t)0;
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)cand;   /* store the term   */
        }
    }
    return terms[n - 1];
}
