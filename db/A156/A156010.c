/*
 * A156010  Primes p such that 2*p+3 and 8*p+21 are both prime.
 * https://oeis.org/A156010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p such that 2*p+3 and 8*p+21 are also
 *            prime
 * Range:     u8 n <= 12, u16 n <= 381, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* External linkage – as required. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1; return 0. */
        return (seq_t)0;

    /* Static cache of found terms. */
    static seq_t *terms      = NULL;   /* array of results                */
    static size_t term_cap   = 0;       /* allocated capacity              */
    static size_t term_len   = 0;       /* number of terms computed        */

    /* Next candidate to test (prime). */
    static idx_t next_candidate = 2;    /* start with the first prime      */

    /* Ensure enough space to hold at least n terms. */
    if (term_len < n) {
        /* Grow capacity if needed for the current terms. */
        if (term_cap < term_len + 1) {
            size_t new_cap = term_cap ? term_cap * 2 : 128;
            if (new_cap < term_len + 1) new_cap = term_len + 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            terms = tmp;
            term_cap = new_cap;
        }

        /* Find additional terms until we have n of them. */
        while (term_len < n) {
            if (is_prime(next_candidate)) {
                idx_t v2 = next_candidate * 2 + 3;
                idx_t v8 = next_candidate * 8 + 21;
                if (is_prime(v2) && is_prime(v8)) {
                    /* Grow capacity if the array is full. */
                    if (term_len == term_cap) {
                        size_t new_cap = term_cap ? term_cap * 2 : 128;
                        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                        terms = tmp;
                        term_cap = new_cap;
                    }
                    terms[term_len] = (seq_t)next_candidate;
                    ++term_len;
                }
            }
            ++next_candidate;
        }
    }

    return terms[n - 1];
}
