/*
 * A216509  Primes which cannot be written in the form a^2 + 6*b^2.
 * https://oeis.org/A216509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes which cannot be written in the form a^2 + 6*b^2
   (i.e. primes congruent to 5, 11, 13, 17, 19, 23 modulo 24, together with 2 and 3). */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Static data retained between calls */
    static seq_t *terms = NULL;
    static size_t term_cap = 0;
    static idx_t term_cnt = 0;          /* number of terms already computed */
    static uint64_t next_candidate = 2; /* next integer to test for primality */

    /* return already computed term if available */
    if (n <= term_cnt) {
        return terms[n - 1];
    }

    /* generate terms until we reach the requested index */
    while (term_cnt < n) {
        /* find the next prime ≥ next_candidate */
        uint64_t p = next_candidate;
        for (;;) {
            if (p == 2 || ((p & 1U) && is_prime_small((seq_t)p))) {
                break; /* p is prime */
            }
            ++p;     /* try next integer */
        }

        /* check congruence condition */
        if (p == 2 || p == 3 ||
            (p % 24ULL == 5ULL) ||
            (p % 24ULL == 11ULL) ||
            (p % 24ULL == 13ULL) ||
            (p % 24ULL == 17ULL) ||
            (p % 24ULL == 19ULL) ||
            (p % 24ULL == 23ULL)) {
            /* store the term */
            ensure_capacity(term_cnt + 1, &term_cap,
                            (void **)&terms, sizeof(seq_t));
            terms[term_cnt++] = (seq_t)p;
        }

        /* prepare for next search */
        next_candidate = p + 1;
    }

    /* now the requested term is available */
    return terms[n - 1];
}
