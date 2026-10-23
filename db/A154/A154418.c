/*
 * A154418  Primes p such that (p^2 + 4)/5 is prime.
 * https://oeis.org/A154418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 647, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that (p^2 + 4)/5 is prime.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t term_cnt = 0;   /* number of terms stored */
    static size_t term_cap = 0;   /* allocated capacity */

    /* next odd number to test for primality */
    static uint64_t cand = 3;

    /* keep generating terms until we have at least n of them */
    while (term_cnt < n) {
        /* find the next prime candidate */
        while (!is_prime((idx_t)cand))
            cand += 2;               /* skip even numbers */

        /* prime found in cand */
        uint64_t p = cand;

        /* only primes p ≡ 1 or 4 (mod 5) can satisfy the condition */
        if (p >= 19 && (p % 5 == 1 || p % 5 == 4)) {
            /* compute q = (p^2 + 4) / 5 in 128‑bit to avoid overflow */
            unsigned __int128 sq  = (unsigned __int128)p * p;
            uint64_t        q    = (uint64_t)((sq + 4) / 5);

            if (is_prime((idx_t)q)) {
                /* ensure we have room for another term */
                if (term_cnt == term_cap) {
                    size_t new_cap = term_cap ? term_cap * 2 : 16;
                    terms = seq_realloc(terms, term_cap, new_cap);
                    term_cap = new_cap;
                }
                terms[term_cnt++] = (seq_t)p;
            }
        }

        cand += 2;   /* advance to next odd number */
    }

    /* requested term is 1‑based */
    return terms[n - 1];
}
