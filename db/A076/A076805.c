/*
 * A076805  Triskaidekaphobic or 13-free primes: primes that do not contain
 *          the number 13.
 * https://oeis.org/A076805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime that does not contain the decimal
 *            substring "13"
 * Range:     u8 n <= 49, u16 n <= 4525, u32 n <= 5656, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains13_uint64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates primes on demand, skipping any prime whose decimal
 * representation contains "13".  The results are memoized so that subsequent
 * calls for arbitrary n work in O(1) after the initial search. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/contains13_uint64.h"

seq_t a(idx_t n) {
    static seq_t *terms = NULL;    /* memoized list of found terms */
    static size_t term_count = 0;  /* number of stored terms */
    static size_t term_cap   = 0;  /* current allocation capacity */
    static uint64_t next_candidate = 2;   /* next integer to test */

    /* Expand storage as needed. */
    if (term_count >= n) return terms[n - 1];

    while (term_count < n) {
        /* Find the next prime that does not contain 13. */
        for (;;) {
            seq_t cand = (seq_t)next_candidate;

            if (is_prime_big(cand) && !contains13_uint64(next_candidate)) {
                /* Make room for the new term. */
                if (term_count >= term_cap) {
                    term_cap = term_cap == 0 ? 16 : term_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    terms = tmp;
                }
                terms[term_count++] = cand;
                ++next_candidate;      /* prepare for next search */
                break;
            }
            ++next_candidate;
        }
    }
    return terms[n - 1];
}
