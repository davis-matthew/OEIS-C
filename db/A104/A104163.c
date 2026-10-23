/*
 * A104163  Primes p such that (2p+1)/3 is prime.
 * https://oeis.org/A104163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p with (2p+1)/3 also prime (p > 0)
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Sequence generator for OEIS A104163 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1 – value for n<1 is undefined */

    /* Memoised list of found terms */
    static seq_t *terms = NULL;
    static idx_t term_cap = 0;    /* allocated size of terms array   */
    static idx_t term_cnt = 0;    /* number of terms stored so far  */
    static uint64_t next_trial = 2;   /* candidate prime to test        */

    /* If we are asked for a term we have not yet computed, extend the list */
    if (n > term_cnt) {
        /* Initialise the array if needed */
        if (terms == NULL) {
            term_cap = 64;
            terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        }

        /* Find terms until we have at least n of them */
        while (term_cnt < n) {
            /* Scan forward until we hit a prime p */
            do { next_trial++; } while (!is_prime((uint64_t)next_trial));

            seq_t p = (seq_t)next_trial;

            /* Compute q = (2p+1)/3 – only consider when the division is exact */
            seq_t two_p_plus_one = (seq_t)2 * p + 1;
            if ((two_p_plus_one % (seq_t)3) != 0) continue;

            seq_t q = two_p_plus_one / (seq_t)3;
            if (!is_prime((uint64_t)q)) continue;     /* q must be prime */

            /* Store the found prime p */
            if (term_cnt == term_cap) {
                term_cap <<= 1;
                terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_cnt++] = p;
        }
    }

    /* Return the requested term (indices are 1‑based) */
    return terms[n - 1];
}
